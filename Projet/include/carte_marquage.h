#pragma once

#include <ostream>
#include "enums.h"
#include "position.h"
#include "tuile.h"
#include "env_joueur.h"
#include <unordered_map>
#include <unordered_set>
#include <memory>


//Fonction de hashage pour Position
namespace std {
	template <>
	struct hash<Position> {
		size_t operator()(const Position& coord) const {
			//on n'utilise que q et r pcq s=-q-r
			size_t h1 = std::hash<int>{}(coord.getQ()); //donne un hash pour q
			size_t h2 = std::hash<int>{}(coord.getR()); //donne un hash pour r
			return h1 ^ (h2 << 1); //combiner deux valeurs hash avec XOR
		}
	};
}

// Classe abstraite de base
class CarteMarquage {
public:
    virtual ~CarteMarquage() = default;

    // Calcul du score avec un environnement commun
    virtual int CalculScore(const EnvJoueur& envJ) const = 0;
};

// --- Déclaration des classes filles ---

// Saumon A, B, C
class CarteSaumonA : public CarteMarquage {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
private:
    int explorerChaine(const std::unordered_map<Position, Tuile>& carte,
        const Position& position,
        std::unordered_set<Position>& PositionsVisitees,
        const Position* parent) const;
};

class CarteSaumonB : public CarteMarquage {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
private:
    int explorerChaine(const std::unordered_map<Faune, std::unordered_set<Position>>& carte,
        const Position& position,
        std::unordered_set<Position>& visited,
        const Position* parent) const;
};

class CarteSaumonC : public CarteMarquage {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

// Ours A, B, C
class CarteOursA : public CarteMarquage {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteOursB : public CarteMarquage {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteOursC : public CarteMarquage {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

// Buse A, B, C
class CarteBuseA : public CarteMarquage {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteBuseB : public CarteMarquage {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteBuseC : public CarteMarquage {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

// Renard A, B, C
class CarteRenardA : public CarteMarquage {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteRenardB : public CarteMarquage {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteRenardC : public CarteMarquage {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

// Wapiti A, B, C
class CarteWapitiA : public CarteMarquage {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
private:
    int explorerChaine(const std::unordered_map<Position, Tuile>& carte,
        const Position& pos,
        std::unordered_set<Position>& visited,
        std::optional<Direction> direction) const;
};

class CarteWapitiB : public CarteMarquage {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

class CarteWapitiC : public CarteMarquage {
public:
    int CalculScore(const EnvJoueur& envJ) const override;
};

/// Factory avec une définition inline
class CarteMarquageFactory {
public:
    static std::unique_ptr<CarteMarquage> creerCarte(const std::string& nom) {
        // Saumon
        if (nom == "SaumonA") return std::make_unique<CarteSaumonA>();
        else if (nom == "SaumonB") return std::make_unique<CarteSaumonB>();
        else if (nom == "SaumonC") return std::make_unique<CarteSaumonC>();

        // Ours
        else if (nom == "OursA") return std::make_unique<CarteOursA>();
        else if (nom == "OursB") return std::make_unique<CarteOursB>();
        else if (nom == "OursC") return std::make_unique<CarteOursC>();

        // Buse
        else if (nom == "BuseA") return std::make_unique<CarteBuseA>();
        else if (nom == "BuseB") return std::make_unique<CarteBuseB>();
        else if (nom == "BuseC") return std::make_unique<CarteBuseC>();

        // Renard
        else if (nom == "RenardA") return std::make_unique<CarteRenardA>();
        else if (nom == "RenardB") return std::make_unique<CarteRenardB>();
        else if (nom == "RenardC") return std::make_unique<CarteRenardC>();

        // Wapiti
        else if (nom == "WapitiA") return std::make_unique<CarteWapitiA>();
        else if (nom == "WapitiB") return std::make_unique<CarteWapitiB>();
        else if (nom == "WapitiC") return std::make_unique<CarteWapitiC>();

        else throw std::invalid_argument("Type de carte inconnu : " + nom);
    }
};

/*  BEGINNING OLD VERSION

// Classe abstraite avec méthode virtuelle pure methode calcul
class CarteMarquage {
	Faune faune;
public:
	CarteMarquage(Faune f) : faune(f) {}
	virtual int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const = 0;
	virtual int methodeCalculB(const std::unordered_map<Faune, std::unordered_set<Position>>& carte) const = 0;
	virtual int methodeCalculC(const std::unordered_map<Faune, std::unordered_set<Position>>& carte) const = 0;
	virtual int methodeCalculD(const std::unordered_map<Faune, std::unordered_set<Position>>& carte) const = 0;
	Faune getFaune() const { return faune; }
	virtual ~CarteMarquage() = default;
};

class CarteSaumon : public CarteMarquage {
public:
	CarteSaumon() : CarteMarquage(Faune::saumon) {}
	int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const override;
	int explorerChaineSaumonA(const std::unordered_map<Position, Tuile>& carte, const Position& position, std::unordered_set<Position>& positionsVisitees, const Position* pere) const;
	int methodeCalculB(const std::unordered_map<Faune, std::unordered_set<Position>>& carte) const override;
	int explorerChaineSaumonB(const std::unordered_map<Faune, std::unordered_set<Position>>& carte,
		const Position& position,
		std::unordered_set<Position>& positionsVisitees,
		const Position* pos_pere) const;
};

class CarteOurs : public CarteMarquage {
public:
	CarteOurs() : CarteMarquage(Faune::ours) {}
	int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const override;
	int methodeCalculB(const std::unordered_map<Faune, std::unordered_set<Position>>& carte) const;
};

class CarteBuse : public CarteMarquage {
public:
	CarteBuse() : CarteMarquage(Faune::buse) {}
	int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const override;
	int methodeCalculB(const std::unordered_map<Faune, std::unordered_set<Position>>& carte) const;

};

class CarteRenard : public CarteMarquage {
public:
	CarteRenard() : CarteMarquage(Faune::renard) {}
	int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const override;
	int methodeCalculB(const std::unordered_map<Faune, std::unordered_set<Position>>& carte) const;
};

class CarteWapiti : public CarteMarquage {
public:
	CarteWapiti() : CarteMarquage(Faune::wapiti) {}
	int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const override;
	int explorerChaineWapitiA(const std::unordered_map<Position, Tuile>& carte, const Position& position, std::unordered_set<Position>& PositionsVisitees, std::optional<Direction> direction) const;
	int methodeCalculB(const std::unordered_map<Faune, std::unordered_set<Position>>& carte) const;
};




END OLD VERSION
*/