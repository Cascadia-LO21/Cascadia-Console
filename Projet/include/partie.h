#pragma once

#include <vector>
#include <memory>
#include <stdexcept>
#include <ostream>
#include <optional>
#include "enums.h"
#include "env_joueur.h"
#include "pioche.h"
#include "gestion_pieces.h"

class Partie {
private:
    static constexpr int MAX_NB_TOURS = 20; // valeur par défaut officielle
    static constexpr int MAX_NB_JOUEURS = 4;
    int nbJoueurs;
    int compteurTour;
    std::vector<EnvJoueur> joueurs;
    int joueurCourant;
    bool pause;
    std::unique_ptr<Pioche> pioche;
    Marquage marquage;
    Variante variante;
    std::optional<std::vector<int>> gagnant; // pour gerer les cas d'égalité
    std::optional<std::vector<int>> scores;

    void initialiserPioche(); // privé, car on ne souhaite pas laisser quiconque initialiser la pioche

public:

    // TODO : reviser le constructeur pour repondre au cosntructeur de Pioche si elle a besoin de param
    // La pioche est initialisée en même temps que Partie.
    // Mais le vecteur de joeur est vide encore.
    Partie(int nombreJoueurs = 1) :
        nbJoueurs(nombreJoueurs), compteurTour(0), joueurs(), joueurCourant(0), pause(false),
        pioche(std::make_unique<Pioche>()),
        marquage(Marquage::A), variante(Variante::standard) {}

 
    void setNbJoueurs(int i = 1) {
        if (i >= 1 && i <= MAX_NB_JOUEURS) 
            nbJoueurs = i;
        else 
            throw std::out_of_range("Le nombre de joueurs va de 1 à " + std::to_string(MAX_NB_JOUEURS));
    }

    void ajouterJoueur(const EnvJoueur& joueur);
    void ajouterJoueur(const std::string& nom);
    void setMarquage(Marquage m = Marquage::A) { marquage = m; }
    void setVariante(Variante v = Variante::standard) { variante = v; }

  
    int getNbJoueurs() const { return nbJoueurs; }
    int getNbTours() const { return MAX_NB_TOURS; }
    int getIndexJoueurCourant() const{ return joueurCourant; }
    const EnvJoueur& getEnvJoueurCourant() const { return joueurs.at(joueurCourant); }
    const std::vector<EnvJoueur>& getJoueurs() const { return joueurs; }
    void afficheJoueurs() const;
    Marquage getMarquage() const { return marquage; }
    Variante getVariante() const { return variante; }
    const Pioche& getPioche() const { return *pioche; }
    void affichePioche() const;


    void lancer();
    void prochainJoueur() { joueurCourant = (joueurCourant + 1) % nbJoueurs; }
    void pause() { if (!pause) pause = true; }
    void reprendre() { if (pause) pause = false; }
    void reset(); 


    void jouerTour(); // un tour fait jouer tous les joueurs
    bool verifierFinPartie() const; // d'apres les regles du jeu, c'est quand la pile des tuiles est vide
    void calculerScores();
    void afficherScores() const;
    void calculerGagnant();
    void afficherGagnant() const;
};

std::ostream& operator<<(std::ostream& os, const Partie& partie);
