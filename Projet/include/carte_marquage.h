#pragma once

#include <ostream>
#include "enums.h"
#include "position.h"
#include "tuile.h"

/// Classe abstraite avec méthode virtuelle pure methode calcul
class CarteMarquage {
	Faune faune;
public:
	CarteMarquage(Faune f) : faune(f) {}
	virtual int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const = 0;
	Faune getFaune() const { return faune; }
	virtual ~CarteMarquage() = default; //a redefinir dans classes filles
};

class CarteSaumon : public CarteMarquage {
public:
	CarteSaumon() : CarteMarquage(Faune::saumon) {}
	int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const override;
	//int calculerScore(const unordered_map<Position, Tuile>& map) const;
};

class CarteOurs : public CarteMarquage {
public:
	CarteOurs() : CarteMarquage(Faune::ours) {}
	int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const override;
};

class CarteBuse : public CarteMarquage {
public:
	CarteBuse() : CarteMarquage(Faune::buse) {}
	int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const override;

};

class CarteRenard : public CarteMarquage {
public:
	CarteRenard() : CarteMarquage(Faune::renard) {}
	int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const override;
};

class CarteWapiti : public CarteMarquage {
public:
	CarteWapiti() : CarteMarquage(Faune::wapiti) {}
	int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const override;
};

/* A GARDER JUSQU'À AVOIR FINI TOUTES LES FONCTIONS :

BUSE : 
	switch (count) {
	case 0: return 0;
	case 2: return 5;
	case 3: return 8;
	case 4: return 11;
	case 5: return 14;
	case 6: return 18;
	case 7: return 22;
	default: return 26; //8+ buses
	}

RENARD :
	switch (count) {
	case 0: return 0;
	case 1: return 1;
	case 2: return 2;
	case 3: return 3;
	case 4: return 4;
	default: return 5; //5 et plus
	}

WAPITI :
	switch (count) {
	case 0: return 0;
	case 1: return 2;
	case 2: return 5;
	case 3: return 9;
	default:
		return 13; //4 et plus
	}

OURS : c'est bon

SAUMON : jsp

*/