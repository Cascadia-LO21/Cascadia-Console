#pragma once

#include <ostream>
#include "enums.h"
#include "position.h"
#include "tuile.h"
#include "env_joueur.h"
#include <unordered_map>
#include <unordered_set>
#include <memory>

// Classe abstraite 
class CarteMarquageStandard {
public:
    virtual ~CarteMarquageStandard() = default;

    // Calcul du score avec un environnement commun
    virtual int CalculScore(const EnvJoueur& envJ) const = 0;
};

// --- Déclaration des classes filles ---

// Saumon A, B, C
class CarteSaumonA : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
private:
    int explorerChaine(const std::unordered_map<Position, Tuile>& carte,
        const Position& position,
        std::unordered_set<Position>& PositionsVisitees,
        const Position* parent) const;
};

class CarteSaumonB : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
private:
    int explorerChaine(const std::unordered_map<Faune, std::unordered_set<Position>>& carte,
        const Position& position,
        std::unordered_set<Position>& visited,
        const Position* parent) const;
};

class CarteSaumonC : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
    int explorerChaine(const std::unordered_map<Faune, std::unordered_set<Position>>& carte,
        const Position& position,
        std::unordered_set<Position>& visited,
        const Position* parent) const;
};

class CarteSaumonD : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
    int explorerChaine(const std::unordered_map<Faune, std::unordered_set<Position>>& carte,
        const Position& position,
        std::unordered_set<Position>& visited,
        const Position* parent) const;
};

// Ours A, B, C
class CarteOursA : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteOursB : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteOursC : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteOursD : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

// Buse A, B, C
class CarteBuseA : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteBuseB : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteBuseC : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteBuseD : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

// Renard A, B, C
class CarteRenardA : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteRenardB : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteRenardC : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteRenardD : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

// Wapiti A, B, C, D
class CarteWapitiA : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
private:
    int explorerChaine(const std::unordered_map<Position, Tuile>& carte,
        const Position& pos,
        std::unordered_set<Position>& visited,
        std::optional<Direction> direction) const;
};

class CarteWapitiB : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteWapitiC : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteWapitiD : public CarteMarquageStandard {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};



// VARIANTES
class CarteMarquageVariante {
public:
    virtual ~CarteMarquageVariante() = default;
    virtual std::unordered_map<Faune, unsigned int> CalculScore(const EnvJoueur& envJ) const = 0;
};

class VarianteFamiliale : public CarteMarquageVariante {
public:
    std::unordered_map<Faune, unsigned int> CalculScore(const EnvJoueur& envJ) const override;
};

class VarianteIntermediaire : public CarteMarquageVariante {
public:
    std::unordered_map<Faune, unsigned int> CalculScore(const EnvJoueur& envJ) const override;
};



// Factory 1
class CarteMarquageStandardFactory {
public:
    static std::unique_ptr<CarteMarquageStandard> creerCarte(const std::string& nom) {
        // Saumon
        if (nom == "saumonA") return std::make_unique<CarteSaumonA>();
        else if (nom == "saumonB") return std::make_unique<CarteSaumonB>();
        else if (nom == "saumonC") return std::make_unique<CarteSaumonC>();

        // Ours
        else if (nom == "oursA") return std::make_unique<CarteOursA>();
        else if (nom == "oursB") return std::make_unique<CarteOursB>();
        else if (nom == "oursC") return std::make_unique<CarteOursC>();

        // Buse
        else if (nom == "buseA") return std::make_unique<CarteBuseA>();
        else if (nom == "buseB") return std::make_unique<CarteBuseB>();
        else if (nom == "buseC") return std::make_unique<CarteBuseC>();

        // Renard
        else if (nom == "renardA") return std::make_unique<CarteRenardA>();
        else if (nom == "renardB") return std::make_unique<CarteRenardB>();
        else if (nom == "renardC") return std::make_unique<CarteRenardC>();

        // Wapiti
        else if (nom == "wapitiA") return std::make_unique<CarteWapitiA>();
        else if (nom == "wapitiB") return std::make_unique<CarteWapitiB>();
        else if (nom == "wapitiC") return std::make_unique<CarteWapitiC>();

        else throw std::invalid_argument("Type de carte inconnu : " + nom);
    }
};

// Factory 2
class CarteMarquageVarianteFactory {
public: 
    static std::unique_ptr<CarteMarquageVariante> creerCarte(const std::string& nom) {

        // Variante famille
        if (nom == "famille") return std::make_unique<VarianteFamiliale>();

        // Variante intermédiaire
        else if (nom == "intermediaire") return std::make_unique<VarianteIntermediaire>();

        throw std::invalid_argument("Type de carte inconnu : " + nom);
    }
};


void testCartes();