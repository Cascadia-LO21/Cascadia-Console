#pragma once

#include <vector>
#include <memory>
#include <stdexcept>
#include <ostream>
#include <optional>
#include "enums.h"
#include "env_joueur.h"
#include "pioche.h"

class Partie {
private:
    int nbJoueurs;
    int nbTours;
    std::vector<EnvJoueur> joueurs;
    int joueurCourant;
    bool pause;
    std::unique_ptr<Pioche> pioche;
    Marquage marquage;
    Variante variante;
    std::optional<std::vector<int>> gagnant; // pour gerer les cas d'égalité

    void initialiserPioche();

public:

    Partie();

    // Configuration partie
    void setNbTours(int i = 20);
    void setNbJoueurs(int i = 2);
    void ajouterJoueur(const EnvJoueur& joueur);
    void setMarquage(bool aleatoire = false);
    void setVariante(bool aleatoire = false);

    // Accès aux données
    int getNbJoueurs() const;
    int getNbTours() const;
    const EnvJoueur& getJoueurCourant() const;
    Marquage getMarquage() const;
    Variante getVariante() const;
    const Pioche& getPioche() const;

    // Contrôle flux jeu
    void demarrer();
    void passerTour();
    void pause();
    void reprendre();
    void reset();

    // Logique jeu
    void jouer();
    bool verifierFinPartie() const;
    std::vector<int> calculerGagnant() const;

    void afficherScores() const;

};

std::ostream& operator<<(std::ostream& os, const Partie& partie);
