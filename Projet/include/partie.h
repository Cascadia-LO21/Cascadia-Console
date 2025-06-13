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
#include <fstream>
#include "json.hpp"
//using json = nlohmann::json;
#include "serializers.h"

class Partie {
private:
    //static constexpr unsigned int MAX_NB_TOURS = 20; // valeur par défaut officielle
    static constexpr unsigned int MAX_NB_TOURS = 2; // testing purpose only
    static constexpr unsigned int MAX_NB_JOUEURS = 4;
    unsigned int nbJoueurs;
    unsigned int compteurTour;
    std::vector<EnvJoueur> joueurs;
    unsigned int joueurCourant;
    bool pause;
    std::unique_ptr<Pioche> pioche;
    Marquage marquage;
    Variante variante;

    void initialiserPioche(unsigned int nbJoueur = 1); // privé, car on ne souhaite pas laisser quiconque initialiser la pioche

public:
    // pour la sauvegarde : la classe devient serialisable
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Partie, MAX_NB_TOURS, MAX_NB_JOUEURS, nbJoueurs, compteurTour, joueurs,
        joueurCourant, pause, pioche, marquage, variante)


    // La pioche est vide encore
    // Le vecteur de joueurs est vide encore
    Partie() :
        nbJoueurs(0), compteurTour(0), joueurs{}, joueurCourant(0), pause(false),
        pioche(nullptr),
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

  
    unsigned int getNbJoueurs() const { return nbJoueurs; }
    unsigned int getCompteurTour() const { return compteurTour; }
    unsigned int getIndexJoueurCourant() const{ return joueurCourant; }
    const EnvJoueur& getEnvJoueurCourant() const { return joueurs.at(joueurCourant); }
    EnvJoueur& getEnvJoueurCourantModifiable() { return joueurs.at(joueurCourant); }
    const std::vector<EnvJoueur>& getJoueurs() const { return joueurs; }
    Marquage getMarquage() const { return marquage; }
    Variante getVariante() const { return variante; }
    const Pioche& getPioche() const { return *pioche; }
    Pioche& getPiocheModifiable() { return *pioche; }
    unsigned int getMaxNbJoueurs() const { return MAX_NB_JOUEURS; }
    unsigned int getMaxNbTours() const { return MAX_NB_TOURS; }
    bool getPause() const { return pause; }

    void incCompteurTour() { compteurTour++; }

    std::vector<EnvJoueur>& getJoueursModifiable() { return joueurs; }

    void preparer();
    void prochainJoueur() { joueurCourant = (joueurCourant + 1) % nbJoueurs; }
    void pauser() { if (!pause) pause = true; }
    void reprendre() { if (pause) pause = false; }
    void reset(); 


    //void jouerTourIndividuel();
    //void jouerTourCollectif(); // un tour fait jouer tous les joueurs
    bool verifierFinPartie() const; // d'apres les regles du jeu, c'est quand la pile des tuiles est vide

    void revenir(unsigned int indexTuile, unsigned int indexJeton);
    void apresPlacementDefinitif(EnvJoueur& player);
};