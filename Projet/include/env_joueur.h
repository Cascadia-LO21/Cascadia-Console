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
	
	std::array<int, 5> scoreHabitat;
	std::array<int, 5> scoreFaune;
	unsigned int nbJetonNature;
	unsigned int scoreFinal;

	// Undo system
	std::optional<Position> dernierePosition; //la dernière position jouée
	std::optional<Tuile> derniereTuile; // derniere tuile placee
	bool placementEnAttente = false; //est ce qu'il y a un placement en attente de confirmation?

public:

	//constructeur principal
	EnvJoueur(const std::string& pseudo, unsigned int nbJetonNature = 0, unsigned int scoreFinal = 0)
		: pseudo(pseudo), nbJetonNature(nbJetonNature), scoreFinal(scoreFinal), scoreHabitat{}, scoreFaune{} {

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
	void placerTuile(const Position& coord, Tuile& tuile);

	//étape 2: confirmer le placement de la tuile
	void confirmerPlacement();

	//annuler le dernier placement
	bool undoDernierPlacement();

	////alternative combine étapes 1 et 2, placer une tuile et confirmer son placement
	void placerTuileDefinitive(const Position& pos, Tuile& tui);

	//=============fin systeme placement tuile + undo
	
	//pour placer un jeton faune, on ne peut pas undo cette opération
	int placerJetonFaune(const Position& coord, const JetonFaune& jeton);

	void setTuilesDepart(std::vector<Tuile>& tuilesDepart);
	void placerTuileDepart(const Position& coord, Tuile& tuile);
	void placerTuileDefinitiveDepart(const Position& coord, Tuile& tuile);

	void incNbJetonsNature() { nbJetonNature++; };
	void decNbJetonsNature() { nbJetonNature--; };
	
	//retourne un vecteur de positions possibles pour placer une tuile
	std::vector<Tuile> getTuilesAvecVoisinLibre() const;
	std::vector<Direction> getDirLibresAutourTuile(const Tuile& tuile) const;
	std::vector<Position> getPosLibresAutourTuile(const Tuile& tuile) const;
	std::vector<Position> getPosLibresPourJeton(Faune f) const;

	//GETTERS & SETTERS
	std::string getPseudo() const { return pseudo; };
	void setPseudo(const std::string& pseudo) {
		this->pseudo = pseudo;
	};
	unsigned int getScore() const { return scoreFinal; };
	void setScore(unsigned int score) { scoreFinal = score; };
	unsigned int getNbJetonsNature() const { return nbJetonNature; };
	std::optional<Position> getDernierePosition() const { return dernierePosition; };
	std::optional<Tuile> getDerniereTuile() const { return derniereTuile; };

	const std::unordered_map<Position, Tuile>& getTuiles() const { return tuiles; }
	const std::unordered_map<Faune, std::unordered_set<Position>>& getMapPositionsJetons() const { return mapPositionsJetons; }
	bool getPlacementEnAttente() const { return placementEnAttente; }


	void setScoreHabitat() { scoreHabitat = std::array<int, 5>{}; }
	std::array<int, 5> getScoreHabitat() { return scoreHabitat; }

	void setScoreFaune() { scoreFaune = std::array<int, 5>{}; }
	std::array<int, 5> getScoreFaune() { return scoreFaune; }
};


//tests
void testHexagonalDisplay();
void testDivers();
void testEnvJoueur();