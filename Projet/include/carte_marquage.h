#pragma once

#include <ostream>
#include "enums.h"
#include "position.h"
#include "tuile.h"
#include <unordered_map>
#include <unordered_set>

//fonction de hashage pour Position
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


/// Classe abstraite avec méthode virtuelle pure methode calcul
class CarteMarquage {
	Faune faune;
public:
	CarteMarquage(Faune f) : faune(f) {}
	virtual int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const = 0;
	virtual int methodeCalculB(const std::unordered_map<Faune, std::unordered_set<Position>>& carte) const = 0;
	virtual int methodeCalculC(const std::unordered_map<Faune, std::unordered_set<Position>>& carte) const = 0;
	virtual int methodeCalculD(const std::unordered_map<Faune, std::unordered_set<Position>>& carte) const = 0;
	Faune getFaune() const { return faune; }
	virtual ~CarteMarquage() = default; //a redefinir dans classes filles
};

class CarteSaumon : public CarteMarquage {
public:
	CarteSaumon() : CarteMarquage(Faune::saumon) {}
	int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const override;
	int explorerChaineSaumonA(const std::unordered_map<Position, Tuile>& carte, const Position& position, std::unordered_set<Position>& positionsVisitees, const Position* pere) const;
	int methodeCalculB(const std::unordered_map<Faune, std::unordered_set<Position>>& carte) const;
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


/*
void testCartesA(const std::unordered_map<Position, Tuile>& carte) ;

int main() {
	std::unordered_map();
	testCartesA(carte);
}
*/