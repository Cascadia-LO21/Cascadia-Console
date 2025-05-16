#pragma once

#include <vector>
#include <stdexcept>
#include <ostream>
#include "enums.h"

class Position {

	int q, r, s;

public:

	Position(int x1 = 0, int x2 = 0, int x3 = 0) : q(x1), r(x2), s(x3) {
		if (q + r + s != 0) {
			throw std::invalid_argument("La somme des coordonnees doivent donner 0. Veuillez les reviser.");
		}
	}

	int getQ() const { return q; }
	int getR() const { return r; }
	int getS() const { return s; }

	std::vector<Position> getVecteurPositionsAdjacentes() const;

	bool estAdjacente(const Position& position) const;

	bool operator==(const Position& position) const;

	bool operator!=(const Position& position) const;

	const Position operator+(const Position& position) const;

	const Position operator-(const Position& position) const;

	const Position getPositionAdjacente(Direction direction) const;

};

std::ostream& operator<<(std::ostream& flux, const Position& p);

std::string directionToString(Direction dir);

extern const std::vector <Position> direction_vecteur;

Direction getDirectionOpposee(Direction dir);

Direction coteTangent(const Position& a, const Position& b);

void testClassePosition();