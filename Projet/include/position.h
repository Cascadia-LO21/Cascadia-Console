#pragma once

#include <vector>
#include <stdexcept>
#include <ostream>
#include <functional>
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

	//donne la position de la tuile trouv� dans cette direction
	const Position getPositionAdjacente(Direction direction) const;
	//donne la direction de la tuile trouv� � cette position
	const Direction getDirectionAdjacente(Position pos) const;

};

std::ostream& operator<<(std::ostream& flux, const Position& p);
std::ostream& operator<<(std::ostream& flux, const std::vector<Position>& v);

std::string directionToString(Direction dir);

extern const std::vector <Position> direction_vecteur;

Direction getDirectionOpposee(Direction dir);

Direction coteTangent(const Position& a, const Position& b);

void testClassePosition();

namespace std {
	template<>
	struct hash<Position> {
		size_t operator()(const Position& pos) const {
			size_t h1 = std::hash<int>{}(pos.getQ());
			size_t h2 = std::hash<int>{}(pos.getR());
			size_t h3 = std::hash<int>{}(pos.getS());
			return h1 ^ (h2 << 1) ^ (h3 << 2); // ou une autre combinaison
		}
	};
}