#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <stack>
#include <algorithm>
#include "partie.h"

// Avant le démarrage d'une partie, la pioche doit être initialiser.
void Partie::initialiserPioche() {
	pioche = std::make_unique<Pioche>();
}

// Ajoute un nouveua EnvJoueur aux vecteur de EnvJoueur/
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

void Partie::jouerTour() {

}

// Controle le deroulement d'une partie.
// Suppose d'avoir defini les joueurs et instancier la pioche pour pouvoir commencer à jouer sereinement.
void Partie::lancer() {
    
    // Verifier si des joueurs existent
    if (joueurs.empty()) {
        throw std::runtime_error("Impossible de démarrer : aucun joueur encore.");
    }

    // S'assurer que les attributs sont dans des états primitifs
    if (nbJoueurs != joueurs.size()) nbJoueurs = joueurs.size();
    if (joueurCourant != 0) joueurCourant = 0;
    if (compteurTour != 0) compteurTour = 0;

    // Vérifier si la pioche est initialisée
    if (!pioche) {
        throw std::runtime_error("Impossible de démarrer : la pioche n'est pas initialisée !");
    }

    // recuperer les tuiles en donnees json
    using namespace GestionPieces; // pour acceder directement à ses fonctions
    std::vector<Tuile> tuilesNonReperes = instancierTuiles("json/tuiles_non_reperes.json");
    std::vector<Tuile> tuilesReperes = instancierTuiles("json/tuiles_reperes.json");
    std::vector<Tuile> tuiles = fusionnerVecteursTuiles(tuilesNonReperes, tuilesReperes);
    melangerTuiles(tuiles);
    std::vector<std::vector<Tuile>> tuilesDepart = instancierTuilesDepart("json/tuiles_depart.json");
    //melangerTuiles(tuilesDepart); // WARNING: surcharge a definir dans GestionPieces

    // creer la pile de Tuiles dispo au sein de la Pioche, en fonction du nbJoueurs
    pioche->setTuilesDispo(tuiles, nbJoueurs);

    // distribuer tuiles de depart à ses joueurs?
    for (int i = 0; i < nbJoueurs; i++) {
        joueurs.at(i).setTuileDepart(tuilesDepart.at(i));
    }

    std::cout << "La partie commence !" << std::endl;

    // Boucle principale du jeu
    while (!verifierFinPartie()) {
        std::cout << "Tour " << (compteurTour + 1) << " / " << MAX_NB_TOURS << std::endl;
        jouerTour();
        compteurTour++;
    }

    // Afficher le résultat final
    calculerScores();
    afficherScores();
    calculerGagnant();
    afficherGagnant();
}

void Partie::reset() {
    joueurs.clear();
    scores = std::nullopt;
    pioche = std::make_unique<Pioche>();
    nbJoueurs = 0;
    compteurTour = 0;
    gagnant = std::nullopt;
    reprendre(); // ou: pause = false;
    marquage = Marquage::A;
    variante = Variante::standard;
    std::cout << "La partie a été réinitialisée." << std::endl;
}

bool Partie::verifierFinPartie() const {
    return pioche->nbTuilesDispo() == 0;
}

// TODO: verifier que les methodes sont bien implementees dans EnvJoueur.
void Partie::calculerScores() {
    if (scores.has_value()) throw std::runtime_error("Des scores existent déjà.");

    scores = std::vector<int>{};
    for (auto& j : joueurs) {
        j.calculScore();
        scores->push_back(j.getScore());
    }
}

// Affiche le score de chaque joueur.
void Partie::afficherScores() const {
    if (!scores.has_value()) 
        std::cout << "Aucun score à afficher" << std::endl;
    else {
        std::cout << "* SCORES *" << std::endl;
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