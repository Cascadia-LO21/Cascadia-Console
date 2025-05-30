#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <stack>
#include <algorithm>
#include "partie.h"
#include "gestion_pieces.h"

// Avant le démarrage d'une partie, la pioche doit être initialiser.
void Partie::initialiserPioche(int nbJoueur) {
	pioche = std::make_unique<Pioche>(nbJoueur);
}

// Ajoute un nouveau EnvJoueur aux vecteur de EnvJoueur
void Partie::ajouterJoueur(const EnvJoueur& joueur) {
    if (joueurs.size() >= nbJoueurs) {
        throw std::runtime_error("Impossible d'ajouter un joueur : nombre maximum atteint (" + std::to_string(nbJoueurs) + ")");
    }
    joueurs.push_back(joueur); // copie du joueur
}

// Cette surcharge permet d'écrire par exemple: ajouterJoueur("toto")
void Partie::ajouterJoueur(const std::string& nom) {
    if (joueurs.size() >= nbJoueurs) {
        throw std::runtime_error("Impossible d'ajouter un joueur : nombre maximum atteint (" + std::to_string(nbJoueurs) + ")");
    }
    joueurs.emplace_back(nom); // creation et ajout d'un EnvJoueur
}

// Affiche les pseudos de tous les joeurs.
void Partie::afficheJoueurs() const {
    if (joueurs.empty())
        throw std::runtime_error("Aucun joueur encore.");
    for (const auto& j : joueurs)
        std::cout << j << std::endl;
}

void Partie::affichePioche() const {
    if (pioche) std::cout << *pioche;
    else std::cout << "La pioche est à créer.";
}

//TODO
void Partie::jouerTourIndividuel() {
    if (nbJoueurs == 1) {
        jouerTourIndividuel();
        pioche->retirerTuileJetonDebut();
    }
}

// TODO
void Partie::jouerTourCollectif() {
    std::cout << "> Début du tour " << compteurTour + 1 << std::endl;

    for (joueurCourant = 0; joueurCourant < nbJoueurs; joueurCourant++) {
        jouerTourIndividuel();
    }


}

// Controle le deroulement d'une partie entiere.
// Suppose d'avoir defini les joueurs et instancier la pioche pour pouvoir commencer à jouer sereinement.
void Partie::lancer() {

    /// 1. Verifications des conditions preliminaires pour que le jeu puisse etre demarre.
    // Verifier si des joueurs existent
    if (joueurs.empty()) {
        throw std::runtime_error("Impossible de démarrer : aucun joueur encore.");
    }

    // S'assurer que les attributs sont dans des états primitifs
    if (nbJoueurs != joueurs.size()) nbJoueurs = joueurs.size();
    if (joueurCourant != 0) joueurCourant = 0;
    if (compteurTour != 0) compteurTour = 0;

    // Vérifier si la pioche est initialisée: si non, on l'initialise alors
    if (!pioche) { initialiserPioche(nbJoueurs); }

    // distribuer une tuile de depart à chaque joueur : celle-ci proviennent de la Pioche, instancié à partir de donnees JSON
    if (nbJoueurs > pioche->getTuilesDepartDispo().size()) // pas assez de tuiles de depart pour le nombre total de joueurs
        throw std::runtime_error(std::to_string(pioche->getTuilesDepartDispo().size()) +
            "tuile de depart pour " + std::to_string(nbJoueurs) + "joueurs : il en manque ! \n" +
            "Veuillez ajouter de nouvelles tuiles de depart ou bien diminuer le nombre de joueurs.");

    for (int i = 0; i < nbJoueurs; i++) {
        //joueurs.at(i).setTuilesDepart(pioche->getTuilesDepartDispo().at(i));
        std::vector<std::vector<Tuile>> tuilesDepart = pioche->getTuilesDepartDispo();
        joueurs.at(i).setTuilesDepart(GestionPieces::piocherTuileDepart(tuilesDepart));
    }

    std::cout << "[JEU CASCADIA]\n";
    std::cout << "La pioche est prete et les tuiles de depart ont ete distribuees a chacun des joueurs.\n";
    std::cout << "La partie peut commencer !\n" << std::endl;

    /// 2. Boucle principale du jeu
    while (!verifierFinPartie()) { // tant qu'il reste des tours à jouer avant d'atteindre NB_MAX_TOURS
        std::cout << "> TOUR " << (compteurTour + 1) << " / " << MAX_NB_TOURS << std::endl;
        jouerTourCollectif();
        compteurTour++;
    }

    /// 3. Afficher le résultat final
    calculerScores();
    afficherScores();
    calculerGagnant();
    afficherGagnant();
    std::cout << "[FIN DU JEU CASCADIA]" << std::endl;
}

void Partie::reset() {
    joueurs.clear();
    scores = std::nullopt;
    pioche = nullptr;
    nbJoueurs = 0;
    compteurTour = 0;
    gagnant = std::nullopt;
    reprendre(); // ou: pause = false;
    marquage = Marquage::A;
    variante = Variante::standard;
    std::cout << "La partie a été réinitialisée." << std::endl;
}

// exemple : avec 20 tours, dès que le compteur fini son 20e tour, le jeu s'arrete
bool Partie::verifierFinPartie() const {
    return compteurTour == MAX_NB_TOURS + 1;
}

// TODO: verifier que les methodes sont bien implementees dans EnvJoueur.
//void Partie::calculerScores() {
//    if (scores.has_value()) throw std::runtime_error("Des scores existent déjà.");
//
//    scores = std::vector<int>{};
//    for (auto& j : joueurs) {
//        j.calculScore();
//        scores->push_back(j.getScore());
//    }
//}

// Affiche le score de chaque joueur.
void Partie::afficherScores() const {
    if (!scores.has_value()) 
        std::cout << "Aucun score à afficher !" << std::endl;
    else {
        std::cout << "[SCORES]" << std::endl;
        for (int i = 0; i < nbJoueurs; i++) {
            std::cout << joueurs.at(i).getPseudo() << " : " << scores->at(i) << std::endl;
        }
    }
}

// Cette fonction permet de stocker les indices du ou des gagnants.
// Elle suppose que les scores de chaque joueur sont deja stockés dans scores,
// grâce à calculerScores()
void Partie::calculerGagnant(){
    if (joueurs.empty()) throw std::runtime_error("Aucun joueur existant.");

    if (scores && compteurTour == MAX_NB_TOURS) {
        gagnant = std::nullopt; // s'assurer que aucun gagnant existe encore
        gagnant = std::vector<int>{}; // initialiser l'attribut optionnel
        int maxScore = *std::max_element(scores->begin(), scores->end());
        for (int i = 0; i < nbJoueurs; i++) {
            if (scores->at(i) == maxScore)
                gagnant->push_back(i);
        }
    } else {
        std::cout << "La partie n'est pas terminée encore." << std::endl;
    }
}

// Affiche le ou les gagnants (en cas d'ex aequo).
// Suppose d'avoir calculerGagnant() au prealable.
void Partie::afficherGagnant() const {
    if (!gagnant)
        throw std::runtime_error("Veuillez calculer les gagnants d'abord, avant d'afficher les gagnants.");
    
    gagnant->size() == 1 ? std::cout << "* GAGNANT * \n" : std::cout << "* GAGNANTS * \n";
    for (auto i : *gagnant) {
        std::cout << joueurs.at(i).getPseudo() << " avec " << scores->at(i) << " pts !" << std::endl;
    }
}