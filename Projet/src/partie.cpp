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
    if (joueurs.size() >= MAX_NB_JOUEURS) {
        throw std::runtime_error("Impossible d'ajouter un joueur : nombre maximum atteint (" + std::to_string(nbJoueurs) + ")");
    }
    joueurs.push_back(joueur); // copie du joueur
    nbJoueurs++;
}

// Cette surcharge permet d'écrire par exemple: ajouterJoueur("toto")
void Partie::ajouterJoueur(const std::string& nom) {
    if (joueurs.size() >= MAX_NB_JOUEURS) {
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
    if (nbJoueurs != joueurs.size()) nbJoueurs = static_cast<unsigned int>(joueurs.size());
    if (joueurCourant != 0) joueurCourant = 0;
    if (compteurTour != 0) compteurTour = 0;

    // pioche prete ?
    initialiserPioche(nbJoueurs); 

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

void Partie::reset() {
    joueurs.clear();
    pioche = nullptr;
    nbJoueurs = 0;
    compteurTour = 0;
    //gagnant = std::nullopt;
    reprendre(); // ou: pause = false;
    marquage = Marquage::A;
    variante = Variante::standard;
    std::cout << "La partie a été réinitialisée." << std::endl;
}

// exemple : avec 20 tours, dès que le compteur fini son 20e tour, le jeu s'arrete
bool Partie::verifierFinPartie() const {
    return compteurTour == MAX_NB_TOURS;
}

void Partie::revenir(unsigned int indexTuile, unsigned int indexJeton) {
    pioche->setVisibilite(indexTuile, true, true); //tuile
    pioche->setVisibilite(indexJeton, true, false); //jeton
    getEnvJoueurCourantModifiable().undoDernierPlacement();
}

void Partie::apresPlacementDefinitif(EnvJoueur& player) {
    player.confirmerPlacement();
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