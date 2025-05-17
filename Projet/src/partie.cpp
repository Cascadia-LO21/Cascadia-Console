#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include "partie.h"

// Avant le démarrage d'une partie, la pioche doit être initialiser.
void Partie::initialiserPioche() {
	pioche = std::make_unique<Pioche>();
}

void Partie::ajouterJoueur(const EnvJoueur& joueur) {
    if (joueurs.size() >= nbJoueurs) {
        throw std::runtime_error("Impossible d'ajouter un joueur : nombre maximum atteint (" + std::to_string(nbJoueurs) + ")");
    }
    joueurs.push_back(joueur); // copie du joueur
}

// Cette surcharge permet d'écrire par exemple: ajouterJoeur("toto")
void Partie::ajouterJoueur(const std::string& nom) {
    if (joueurs.size() >= nbJoueurs) {
        throw std::runtime_error("Impossible d'ajouter un joueur : nombre maximum atteint (" + std::to_string(nbJoueurs) + ")");
    }
    joueurs.emplace_back(nom); // creation et ajout d'un EnvJoueur
}
