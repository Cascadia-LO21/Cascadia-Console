#include <iostream>
#include "position.h"
#include "affichage.h"


Direction getDirectionOpposee(Direction dir) {
	return static_cast<Direction>((static_cast<int>(dir) + 3) % 6);
}

//par exemple, sur le coté NordOuest de a se trouve b, alors la fonction retourne NordOuest
Direction coteTangent(const Position& a, const Position& b)
{
	if (a.estAdjacente(b)) {
		for (int i = 0; i < 6; ++i) {
			Direction dir = static_cast<Direction>(i);
			Position candidate = a.getPositionAdjacente(dir);
			if (candidate == b) return dir;
		}
	}

	return Direction::Inconnue;
}


std::vector<Position> Position::getVecteurPositionsAdjacentes() const {
	return {
		Position(q + 1, r - 1, s),
		Position(q + 1, r, s - 1),
		Position(q, r + 1, s - 1),
		Position(q - 1, r + 1, s),
		Position(q - 1, r, s + 1),
		Position(q, r - 1, s + 1)
	};
}

bool Position::estAdjacente(const Position& position) const {
	const int dq = abs(q - position.q);
	const int dr = abs(r - position.r);
	const int ds = abs(s - position.s);
	return (dq + dr + ds) == 2;
}

bool Position::operator==(const Position& position) const {
	return q == position.q && r == position.r && s == position.s;
}

bool Position::operator!=(const Position& position) const {
	return !(*this == position);
}

const Position Position::operator+(const Position& position) const {
	return Position(q + position.getQ(), r + position.getR(), s + position.getS());
}

const Position Position::operator-(const Position& position) const {
	return Position(q - position.getQ(), r - position.getR(), s - position.getS());
}

const Position Position::getPositionAdjacente(Direction direction) const
{
	const Position& pos = direction_vecteur[static_cast<int>(direction)];
	return Position(q + pos.getQ(), r + pos.getR(), s + pos.getS());
}

Direction Position::getDirectionAdjacente(Position pos) const {
	if (!estAdjacente(pos)) {
		return Direction::Inconnue;
	}
	const std::vector<Position> adjs = getVecteurPositionsAdjacentes();
	for (int i = 0; i < static_cast<int>(adjs.size()); ++i) { //6 voisins pour un hexagone
		if (adjs[i] == pos) {
			return static_cast<Direction>(i);
		}
	}
	return Direction::Inconnue;
}

const std::vector<Position> direction_vecteur = {
	Position(1, -1, 0), //NordEst, 0
	Position(1, 0, -1), //Est, 1
	Position(0, 1, -1), //SudEst, 2
	Position(-1, 1, 0), //SudOuest, 3
	Position(-1, 0, 1), //Ouest,4
	Position(0, -1, 1)  //NordOuest,5
};

void testClassePosition() {
	//test Position
	Position p(1, 0, -1);
	std::cout << p;

	Position q(0, 0, 0);
	std::cout << q << std::endl;

	//cout << p.estAdjacente(q) << endl;

	//cout << (p == q) << endl;
	//cout << (p != q) << endl;
	//
	//cout << p.getPositionsAdjacentes().at(0);

	//cout << (p + q);

	//vector <Position> direction_vecteur = {
	//{+1, -1, 0},
	//{+1, 0, -1},
	//{0, +1, -1},
	//{-1, +1, 0},
	//{-1, 0, +1},
	//{0, -1, +1}
	//};


	//Position est = direction_vecteur[static_cast<int>(Direction::Est)];  
	//int inco = static_cast<int>(Direction::Inconnue);

	//cout << inco << "\n\n";// (+1, -1, 0)

	//cout << (q = direction_vecteur[static_cast<int>(Direction::SudOuest)]);

	//cout << p.getPositionAdjacente(Direction::SudOuest);

	//cout << (direction_vecteur[static_cast<int>(getDirectionOpposee(Direction::NordEst))]);

	//cout << directionToString(Direction::NordEst);

	std::cout << (direction_vecteur[static_cast<int>(coteTangent(p, q))]);
}