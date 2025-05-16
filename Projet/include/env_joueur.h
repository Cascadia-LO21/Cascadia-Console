#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <ostream>
#include "position.h"
#include "tuile.h"
#include "jeton_faune.h"


class EnvJoueur {
	std::string pseudo;
	std::unordered_map<Position, Tuile> tuiles;//la map, pas beosin de désallouer à la fin, on stocke directement les tuiles dans unordered map par valeur
	size_t nbJetonNature;
	size_t scoreFinal;

public:
	void placerTuile(const Position& coord, const Tuile& tuile) {
		tuiles[coord] = tuile;
	};

	//check si ya uen tuile sur cette case
	bool aTuile(const Position& coord) const {
		return tuiles.find(coord) != tuiles.end(); //find trouve la tuile avec la clé coord (renvoie tuiles.end() sinon), end renvoie pointeur vers l'élément après le dernier de tuiles
	};

	//get pointeur de la tuile à cette coordonnée
	const Tuile* getTuile(const Position& coord) const;

	//est ce qu'on peut placer une tuile à cette coordonnée?
	bool positionValide(const Position& coord) const;
};