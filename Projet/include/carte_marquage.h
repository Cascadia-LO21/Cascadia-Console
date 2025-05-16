#pragma once

#include <ostream>
#include "enums.h"
#include "position.h"
#include "tuile.h"

// Classe abstraite avec méthode virtuelle pure
class CarteMarquage {
	Faune faune;
	//TODO : réfléchir à l'implémentation d'un attribut pour quel type de cartes (A,B,C,D) : found.
public:
	CarteMarquage(Faune f) : faune(f) {}
	virtual int methodeCalcul(int count) const = 0;
	Faune getFaune() const { return faune; }
	virtual ~CarteMarquage() = default; 
};

class CarteSaumon : public CarteMarquage {
public:
	CarteSaumon() : CarteMarquage(Faune::saumon) {}
	int methodeCalcul(int count)const override;
};

class CarteOurs : public CarteMarquage {
public:
	CarteOurs() : CarteMarquage(Faune::ours) {}
	int methodeCalcul(int count)const override;
};

class CarteBuse : public CarteMarquage {
public:
	CarteBuse() : CarteMarquage(Faune::buse) {}
	int methodeCalcul(int count)const override;

};

class CarteRenard : public CarteMarquage {
public:
	CarteRenard() : CarteMarquage(Faune::renard) {}
	int methodeCalcul(int count)const override;
};

class CarteWapiti : public CarteMarquage {
public:
	CarteWapiti() : CarteMarquage(Faune::wapiti) {}
	int methodeCalcul(int count)const override;
};