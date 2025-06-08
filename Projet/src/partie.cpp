#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <stack>
#include <algorithm>
#include "partie.h"
#include "gestion_pieces.h"
#include "affichage.h"

// Avant le démarrage d'une partie, la pioche doit être initialiser.
void Partie::initialiserPioche(unsigned int nbJoueur) {
	pioche = std::make_unique<Pioche>(nbJoueur);
}

// Ajoute un nouveau EnvJoueur aux vecteur de EnvJoueur
void Partie::ajouterJoueur(const EnvJoueur& joueur) {
    if (joueurs.size() >= nbJoueurs) {
        throw std::runtime_error("Impossible d'ajouter un joueur : nombre maximum atteint (" + std::to_string(nbJoueurs) + ")");
    }
    joueurs.push_back(joueur); // copie du joueur
    nbJoueurs++;
}

// Cette surcharge permet d'écrire par exemple: ajouterJoueur("toto")
void Partie::ajouterJoueur(const std::string& nom) {
    if (joueurs.size() >= nbJoueurs) {
        throw std::runtime_error("Impossible d'ajouter un joueur : nombre maximum atteint (" + std::to_string(nbJoueurs) + ")");
    }
    joueurs.emplace_back(nom); // creation et ajout d'un EnvJoueur
    nbJoueurs++;
}

// Preparatif avant le demarrage officiel d'une partie
void Partie::preparer() {

    // joueurs prets ?
    if (joueurs.empty()) {
        throw std::runtime_error("Impossible de démarrer : aucun joueur encore.");
    }

    // compteurs prets ?
    if (nbJoueurs != joueurs.size()) nbJoueurs = static_cast<int>(joueurs.size());
    if (joueurCourant != 0) joueurCourant = 0;
    if (compteurTour != 0) compteurTour = 0;

    // pioche prete ?
    if (!pioche) { initialiserPioche(nbJoueurs); }

    // distribuer une tuile de depart à chaque joueur : celle-ci proviennent de la Pioche, instancié à partir de donnees JSON
    if (nbJoueurs > pioche->getTuilesDepartDispo().size()) // pas assez de tuiles de depart pour le nombre total de joueurs
        throw std::runtime_error(std::to_string(pioche->getTuilesDepartDispo().size()) +
            "tuile de depart pour " + std::to_string(nbJoueurs) + "joueurs : il en manque ! \n" +
            "Veuillez ajouter de nouvelles tuiles de depart ou bien diminuer le nombre de joueurs.");

    for (unsigned int i = 0; i < nbJoueurs; i++) {
        std::vector<Tuile> t = GestionPieces::piocherTuileDepart(pioche->getTuilesDepartDispo());
        joueurs.at(i).setTuilesDepart(t);
    }
}

// Controle le deroulement d'une partie entiere.
// Suppose d'avoir defini les joueurs et instancier la pioche pour pouvoir commencer à jouer sereinement.
void Partie::lancer() {



    // TODO: recuperer les fonctions du namespace EvalScore pour calculer les scores
    /// 3. Afficher le résultat final
    //calculerScores();
    //afficherScores();
    //calculerGagnant();
    //afficherGagnant();

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

void Partie::revenir(unsigned int indexTuile, unsigned int indexJeton) {
    pioche->setVisibilite(indexTuile, true); //tuile
    pioche->setVisibilite(indexJeton, false); //jeton
    getEnvJoueurCourantModifiable().undoDernierPlacement();
}

void Partie::apresPlacementDefinitif(EnvJoueur& player) {
    //EnvJoueur& player = getEnvJoueurCourantModifiable();
    //Tuile& tuile = pioche->getTuile(indexTuile);
    player.confirmerPlacement();
    //std::cout << "JAI REUSSI A CONFIRME PLACEMENT\n";
    pioche->slide(0, true); // slide tuiles
    pioche->slide(0, false); // slide jetons

    // Si joueur solitaire, alors son tour est toujours suivi de l'extraction artificielle d'une paire de la pioche
    if (nbJoueurs == 1) {
        pioche->retirerTuileJetonDebut();
        pioche->slide(0, true); // slide tuiles
        pioche->slide(0, false); // slide jetons
    }

    // remplissage de la pioche visible en piochant des pieces de la pioche cachee
    pioche->rafraichirPioche();
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
//void Partie::afficherScores() const {
//    if (!scores.has_value()) 
//        std::cout << "Aucun score à afficher !" << std::endl;
//    else {
//        std::cout << "[SCORES]" << std::endl;
//        for (unsigned int i = 0; i < nbJoueurs; i++) {
//            std::cout << joueurs.at(i).getPseudo() << " : " << scores->at(i) << std::endl;
//        }
//    }
//}
//
//// Cette fonction permet de stocker les indices du ou des gagnants.
//// Elle suppose que les scores de chaque joueur sont deja stockés dans scores,
//// grâce à calculerScores()
//void Partie::calculerGagnant(){
//    if (joueurs.empty()) throw std::runtime_error("Aucun joueur existant.");
//
//    if (scores && compteurTour == MAX_NB_TOURS) {
//        gagnant = std::nullopt; // s'assurer que aucun gagnant existe encore
//        gagnant = std::vector<int>{}; // initialiser l'attribut optionnel
//        int maxScore = *std::max_element(scores->begin(), scores->end());
//        for (unsigned int i = 0; i < nbJoueurs; i++) {
//            if (scores->at(i) == maxScore)
//                gagnant->push_back(i);
//        }
//    } else {
//        std::cout << "La partie n'est pas terminée encore." << std::endl;
//    }
//}
//
//// Affiche le ou les gagnants (en cas d'ex aequo).
//// Suppose d'avoir calculerGagnant() au prealable.
//void Partie::afficherGagnant() const {
//    if (!gagnant)
//        throw std::runtime_error("\nVeuillez calculer les gagnants d'abord, avant d'afficher les gagnants.");
//    
//    gagnant->size() == 1 ? std::cout << "* GAGNANT * \n" : std::cout << "* GAGNANTS * \n";
//    for (auto i : *gagnant) {
//        std::cout << joueurs.at(i).getPseudo() << " avec " << scores->at(i) << " pts !" << std::endl;
//    }
//}
