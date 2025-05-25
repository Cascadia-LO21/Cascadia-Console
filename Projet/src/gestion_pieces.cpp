#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <string>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include "gestion_pieces.h"
#include "enums.h"
#include "tuile.h"
#include "json.hpp"
using json = nlohmann::json;

std::vector<Tuile> GestionPieces::instancierTuiles(const std::string& fichier)
{
	std::ifstream f(fichier);
	if (!f) throw std::runtime_error("Impossible d'ouvrir le fichier JSON : " + fichier);

	std::vector <Tuile> tuiles;

	json donneeJson = json::parse(f);

	for (const auto& tuile : donneeJson["tuiles"]) {

		// Extraction des habitats
		std::array<Habitat, 6> habitats;
		size_t i = 0;
		for (auto habitat : tuile["habitats"]) {
			habitats[i] = stringToHabitat(habitat.get<std::string>());
			i++;
		}
		if (i != 6 || i > habitats.size())
			throw std::runtime_error("Une tuile doit poss�de exactement 6 habitats !");
		
        // Extraction des faunes
		std::vector<Faune> faunes;
		for (auto faune : tuile["faunes"]) {
			faunes.push_back(stringToFaune(faune.get<std::string>()));
		}
		bool donneJetonNature = tuile.value("donneJetonNature", false);
		
        // Construction d'une Tuile directement dans le conteneur tuiles
		tuiles.emplace_back(habitats, faunes, donneJetonNature);
	}
	return tuiles;
}

std::vector<std::vector<Tuile>> GestionPieces::instancierTuilesDepart(const std::string& fichier) {
    std::ifstream file(fichier);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier JSON.");
    }

    json root;
    file >> root;

    if (!root.contains("tuiles_depart") || !root["tuiles_depart"].is_array()) {
        throw std::runtime_error("Format JSON incorrect : 'tuiles_depart' manquant ou non un tableau.");
    }

    std::vector<std::vector<Tuile>> ensembleTripletsDepart;

    for (const auto& triplet : root["tuiles_depart"]) {
        if (!triplet.contains("triplet") || !triplet["triplet"].is_array()) {
            throw std::runtime_error("Format JSON incorrect : 'triplet' manquant ou non un tableau.");
        }

        std::vector<Tuile> tuiles;

        for (const auto& tuile : triplet["triplet"]) {
            if (!tuile.contains("tuile") || !tuile["tuile"].is_object()) {
                throw std::runtime_error("Format JSON incorrect : 'tuile' manquant ou non un objet.");
            }

            const auto& donnee = tuile["tuile"];

            std::array<Habitat, 6> habitats;
            size_t i = 0;
            if (donnee.contains("habitats") && donnee["habitats"].is_array()) {
                for (const auto& habitat : donnee["habitats"]) {
                    if (i < habitats.size()) {
                        habitats[i] = stringToHabitat(habitat.get<std::string>());
                        i++;
                    }
                    else {
                        throw std::runtime_error("Trop d'habitats dans 'tuile'.");
                    }
                }
            }
            else {
                throw std::runtime_error("Format JSON incorrect : 'habitats' manquant ou non un tableau.");
            }

            std::vector<Faune> faunes;
            if (donnee.contains("faunes") && donnee["faunes"].is_array()) {
                for (const auto& faune : donnee["faunes"]) {
                    faunes.push_back(stringToFaune(faune.get<std::string>()));
                }
            }
            else {
                throw std::runtime_error("Format JSON incorrect : 'faunes' manquant ou non un tableau.");
            }

            bool donneJetonNature = donnee.value("donneJetonNature", false);
            tuiles.emplace_back(habitats, faunes, donneJetonNature);
        }

        ensembleTripletsDepart.push_back(std::move(tuiles));
    }

    return ensembleTripletsDepart;
}

std::vector<JetonFaune> GestionPieces::instancierJetonsFaunes()
{
    std::vector<JetonFaune> jetonsFaune;
    std::vector<Faune> fauneTypes = { Faune::saumon, Faune::ours, Faune::buse, Faune::renard, Faune::wapiti };

    // Instancier 20 jetons faune pour chaque type faune, d'apres les regles du jeu
    for (auto type : fauneTypes) {
        for (int i = 0; i < 20; ++i) {
            jetonsFaune.emplace_back(type); 
        }
    }

    return jetonsFaune;
}

// la fonction melanger peut fonctionner avec n’importe quel type T, donc elle peut melanger tous les vecteurs
template<typename T>
void GestionPieces::melanger(std::vector<T>& items)
{
    std::random_device rd;
    std::mt19937 eng(rd());
    std::shuffle(items.begin(), items.end(), eng);
}

void GestionPieces::melangerTuiles(std::vector<Tuile>& tuiles)
{
    melanger(tuiles);
}

void GestionPieces::melangerJetons(std::vector<JetonFaune>& jetons)
{
    melanger(jetons);
}

// melange uniquement la couche exterieure des vecteurs : l'ordre des 3 tuiles dans une tuile de départ est intact
void GestionPieces::melangerTuilesDepart(std::vector<std::vector<Tuile>>& tuiles)
{
    melanger(tuiles); 
}

// la fusion de 2 vecteurs de Tuile appelle les constructeurs par defauts et de recopie de Tuile
std::vector<Tuile> GestionPieces::fusionnerVecteursTuiles(std::vector<Tuile>& v1, std::vector<Tuile>& v2)
{
    std::vector<Tuile> resultat;
    resultat.reserve(v1.size() + v2.size());
    resultat.insert(resultat.end(), v1.begin(), v1.end());
    resultat.insert(resultat.end(), v2.begin(), v2.end());

    // vidons les contenus de v1 et v2 qui sont inutiles a present, vu que leur contenu ont été copié dans resultat
    v1.clear();
    v2.clear();

    return resultat;
}

// selon le nombre de joueurs, nous adpatons la taulle du vecteur contenant toutes les Tuiles. 
// Les Tuile en trop seront tout simplement supprimées par leur destructeur
// Si des places vides sont reservees par le resize, alors elles sont remplis par des Tuiles par défaut
void GestionPieces::adapterTailleVecteurTuiles(std::vector<Tuile>& tuiles, const int nombreJoueurs)
{
    int tailleSouhaitee;
    switch (nombreJoueurs) {
    case 1:
        tailleSouhaitee = 43; // prenons 43, vu que les regles officielles ne le precisent pas
    case 2:
        tailleSouhaitee = 43;
        break;
    case 3:
        tailleSouhaitee = 63;
        break;
    case 4:
        tailleSouhaitee = 83;
        break;
    default:
        std::cerr << "Nombre de joueurs non valide." << std::endl;
        return;
    }
    if (tuiles.size() != tailleSouhaitee) {
        tuiles.resize(tailleSouhaitee);
    }
}

int GestionPieces::nombreTuiles(int nombreJoueurs)
{
    switch (nombreJoueurs) {
    case 1:
         return 43; // prenons 43, vu que les regles officielles ne le precisent pas
    case 2:
        return 43;
    case 3:
        return 63;
    case 4:
        return 83;
    default:
        return MAX_TUILES;
    }
}

// Fait une pile de Tuile, a partir d'un vecteur de Tuile, deja melangé : cela est utile pour la Pioche
std::stack<Tuile> GestionPieces::vectorToStack(std::vector<Tuile>& tuiles)
{
    std::stack<Tuile> pile;
    for (const auto& tuile : tuiles) {
        pile.push(tuile);
    }

    tuiles.clear();

    return pile;
}

// on suppose que le vecteur qui a permis de construire la pile était déjà mélangé
// alors, piocher une Tuile aléatoire, revient à depiler l'élément au sommet de la pile de Tuile
Tuile GestionPieces::piocherTuile(std::stack<Tuile>& pile)
{
    if (pile.empty()) {
        throw std::out_of_range("La pile est vide, aucune tuile a piocher.");
    }
    Tuile tuile = pile.top();
    pile.pop();
    return tuile;
}

// empiler la Tuile sur la pile
void GestionPieces::remettreTuile(std::stack<Tuile>& pile, const Tuile& tuile)
{
    pile.push(tuile);
}

// meme raisonnement que piocherTuile
JetonFaune GestionPieces::piocherJeton(std::vector<JetonFaune>& jetons)
{
    if (jetons.empty()) {
        throw std::out_of_range("Le vecteur de jetons faune est vide, aucun jeton a piocher.");
    }
    JetonFaune jeton = jetons.back();
    jetons.pop_back();
    return jeton;
}

// Une insertion aléatoire dans le vecteur de JetonFaune permet de simuler le sac de JetonFaune
// dans la vie reelle. Cela garantit que la prochain tirage d'un JetonFaune n'est pas forcement
// celui qu'on vient de remettre dans le "sac".
void GestionPieces::remettreJeton(std::vector<JetonFaune>& jetons, const JetonFaune& jeton) {
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<size_t> distr(0, jetons.size());

    size_t randomIndex = distr(eng);
    jetons.insert(jetons.begin() + randomIndex, jeton);
}

std::vector<Tuile> GestionPieces::piocherTuileDepart(std::vector<std::vector<Tuile>>& tuilesDepart)
{
    if (tuilesDepart.empty()) {
        throw std::out_of_range("Il n'y a pas de triplet de tuiles de d�part.");
    }
    std::vector<Tuile> dernierTriplet = tuilesDepart.back();
    tuilesDepart.pop_back();
    return dernierTriplet;
}

void testGestionPieces()
{
    // instancier respectivement les pi�ces du jeu
    std::vector<Tuile> tuiles_reperes = GestionPieces::instancierTuiles("json/tuiles_reperes.json");
    std::vector<Tuile> tuiles_non_reperes = GestionPieces::instancierTuiles("json/tuiles_non_reperes.json");
    std::vector<std::vector<Tuile>> ensemble_tuiles_depart = GestionPieces::instancierTuilesDepart("json/tuiles_depart.json");
    std::vector<JetonFaune> ensemble_jetons_faune = GestionPieces::instancierJetonsFaunes();

    // verification des tailles
    //std::cout << tuiles_reperes.size() << std::endl; 
    //std::cout << tuiles_non_reperes.size() << std::endl; 
    //std::cout << ensemble_tuiles_depart.size() << std::endl;
    //std::cout << ensemble_jetons_faune.size();

    // regroupement
    std::vector<Tuile> ensemble_tuiles = GestionPieces::fusionnerVecteursTuiles(tuiles_reperes, tuiles_non_reperes);
    std::cout << ensemble_tuiles.size() << std::endl;
    //std::cout << tuiles_reperes.size(); // ok, vide
    //for (const auto& t : ensemble_tuiles) std::cout << t << std::endl;


    // melanger
    GestionPieces::melangerTuiles(ensemble_tuiles);
    //std::cout << "Tuiles melangees." << std::endl;
    //std::cout << ensemble_tuiles.size() << std::endl;
    //for (const auto& t : ensemble_tuiles) std::cout << t << std::endl;

    GestionPieces::melangerJetons(ensemble_jetons_faune);
    //std::cout << "Jetons melanges." << std::endl;
    //for (const auto& j : ensemble_jetons_faune) std::cout << j << std::endl;

    GestionPieces::melangerTuilesDepart(ensemble_tuiles_depart);
    //std::cout << "Tuiles de depart melangees." << std::endl;
    //for (const auto& t : ensemble_tuiles_depart) std::cout << t << std::endl;

    // conversion (en pile) + adaptation de taille
    GestionPieces::adapterTailleVecteurTuiles(ensemble_tuiles, 2);
    //std::cout << "Taille des tuiles adaptee : " << ensemble_tuiles.size() << std::endl;
    //for (const auto& t : ensemble_tuiles) std::cout << t << std::endl;

    std::stack<Tuile> pile_tuiles = GestionPieces::vectorToStack(ensemble_tuiles);
    std::cout << "Pile de tuiles creee avec " << pile_tuiles.size() << " tuiles." << std::endl;
    std::cout << ensemble_tuiles.size() << std::endl; //ok, vide, tout est dans la pile maintenant

    //std::cout << std::endl;

    // Piocher une tuile
    //if (!pile_tuiles.empty()) {
    //    std::cout << "Taille de la pile avant tirage : " << pile_tuiles.size() << std::endl;
    //    auto tuile_piochee = GestionPieces::piocherTuile(pile_tuiles);
    //    std::cout << tuile_piochee << std::endl;
    //    std::cout << "Taille de la pile apres tirage : " << pile_tuiles.size() << std::endl;

    //    // Remettre la tuile dans la pile
    //    GestionPieces::remettreTuile(pile_tuiles, tuile_piochee);
    //    std::cout << "Tuile remise dans la pile. Taille de la pile maintenant : " << pile_tuiles.size() << std::endl;
    //}

    //std::cout << std::endl;

    //// Piocher un jeton
    //if (!ensemble_jetons_faune.empty()) {
    //    std::cout << "Taille des jetons avant tirage : " << ensemble_jetons_faune.size() << std::endl;
    //    auto jeton_pioche = GestionPieces::piocherJeton(ensemble_jetons_faune);
    //    std::cout << jeton_pioche << std::endl;
    //    std::cout << "Taille des jetons apres tirage : " << ensemble_jetons_faune.size() << std::endl;

    //    // Remettre le jeton dans le vecteur
    //    GestionPieces::remettreJeton(ensemble_jetons_faune, jeton_pioche);
    //    std::cout << "Jeton remis dans le vecteur. Taille des jetons maintenant : " << ensemble_jetons_faune.size() << std::endl;
    //}

    //std::cout << std::endl;

    // Piocher un triplet de tuiles de depart
    //if (!ensemble_tuiles_depart.empty()) {
    //    std::cout << "Nombre de triplets de tuiles de depart avant tirage : " << ensemble_tuiles_depart.size() << std::endl;
    //    auto triplet_tuiles = GestionPieces::piocherTuileDepart(ensemble_tuiles_depart);
    //    std::cout << triplet_tuiles;
    //    std::cout << "Nombre de triplets de tuiles de depart apres tirage : " << ensemble_tuiles_depart.size() << std::endl;
    //}
}
