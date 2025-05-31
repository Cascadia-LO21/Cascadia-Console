#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "enums.h"
#include "unordered_set"

#include "position.h"
#include "tuile.h"
#include "jeton_faune.h"
#include "pioche.h"


class EnvJoueur {
	std::string pseudo;
	std::unordered_map<Position, Tuile> tuiles;//la map, pas beosin de désallouer à la fin, on stocke directement les tuiles dans unordered map par valeur
	std::unordered_map<Faune, std::unordered_set<Position>> mapPositionsJetons; //map de clé type faune vers un set de positions où le jeton est placé
	
	size_t nbJetonNature;
	size_t scoreFinal;

	// Undo system
	std::optional<Position> dernierePosition; //la dernière position jouée
	std::optional<Tuile> derniereTuile; // derniere tuile placee
	bool placementEnAttente = false; //est ce qu'il y a un placement en attente de confirmation?

public:

	//constructeur principal!!!!!!!!!!!!!!!!TO LOOK AT AGAIN
	EnvJoueur(const std::string& pseudo, size_t nbJetonNature = 0, size_t scoreFinal = 0)
		: pseudo(pseudo), nbJetonNature(nbJetonNature), scoreFinal(scoreFinal) {
	};

	//check si ya uen tuile sur cette case (tuile non confirmée inclus)
	bool aTuile(const Position& coord) const;

	//check si il y a une tuile confirmée à cette position
	bool aTuileConfirmee(const Position& coord) const;

	//est-ce qu'il y a un placement en attente de confirmation?
	bool aPlacementEnAttente() const {
		return placementEnAttente;
	};

	//get pointeur de la tuile à cette coordonnée
	const Tuile* getTuile(const Position& coord) const;

	//get pointeur de la tuile à cette coordonnée version non-const, pour permettre la modification de la tuile
	Tuile* getTuileNonConst(const Position& coord);

	//est ce qu'on peut placer une tuile à cette coordonnée?
	bool positionTuileValide(const Position& coord) const;


	//=============debut systeme placement tuile + undo

	//étape 1: placer une tuile, peut etre undone
	void placerTuile(const Position& coord, const Tuile& tuile);

	//étape 2: confirmer le placement de la tuile
	void confirmerPlacement();

	//annuler le dernier placement
	bool undoDernierPlacement();

	//alternative combine étapes 1 et 2, placer une tuile et confirmer son placement
	void placerTuileDefinitive(const Position& coord, const Tuile& tuile);

	//=============fin systeme placement tuile + undo
	
	//pour placer un jeton faune, on ne peut pas undo cette opération
	int placerJetonFaune(const Position& coord, const JetonFaune& jeton);

	void setTuilesDepart(const std::vector<Tuile>& tuilesDepart);
	void placerTuileDepart(const Position& coord, const Tuile& tuile);
	void placerTuileDefinitiveDepart(const Position& coord, const Tuile& tuile);

	void incNbJetonsNature(size_t nb) { nbJetonNature++; };
	
	//retourne un vecteur de positions possibles pour placer une tuile
	std::vector<Tuile> getTuilesAvecVoisinLibre() const;

	std::vector<Direction> getDirLibresAutourTuile(const Tuile& tuile) const;
	std::vector<Position> getPosLibresAutourTuile(const Tuile& tuile) const;

	//GETTERS & SETTERS
	std::string getPseudo() const { return pseudo; };
	void setPseudo(const std::string& pseudo) {
		this->pseudo = pseudo;
	};
	size_t getScore() const { return scoreFinal; };
	void setScore(size_t score) { scoreFinal = score; };
	size_t getNbJetonsNature() const { return nbJetonNature; };
	std::optional<Position> getDernierePosition() const { return dernierePosition; };
	std::optional<Tuile> getDerniereTuile() const { return derniereTuile; };

	const std::unordered_map<Position, Tuile>& getTuiles() const { return tuiles; }
	const std::unordered_map<Faune, std::unordered_set<Position>>& getMapPositionsJetons() const { return mapPositionsJetons; }
	bool getPlacementEnAttente() const { return placementEnAttente; }

};

std::ostream& operator<<(std::ostream& os, const EnvJoueur& env);

//tests
void testHexagonalDisplay();
void testDivers();
