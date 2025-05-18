#pragma once

#include <vector>
#include <array>
#include <stdexcept>
#include <optional>
#include <memory>
#include <ostream>
#include "position.h"
#include "enums.h"

class Tuile {
	std::array<Habitat, 6> habitats;
	std::vector<Faune> faunes; //un vector est adapté étant donné que le nombre de faunes varient entre 1 et 3
	bool donneJetonNature;
	std::optional<Faune> faunePlace;
	std::unique_ptr<Position> position; //composition
	bool placementConfirme;

public:
	//constructeur principal
	Tuile(const std::array<Habitat, 6>& habitats,
		const std::vector<Faune>& faunes,
		bool nature = false,
		bool jetonPresent = false,
		Position* p = nullptr,
		bool place = false)
		: habitats(habitats), faunes(faunes),
		donneJetonNature(nature), faunePlace(std::nullopt),
		position(p), placementConfirme(place) {
		if (faunes.size() < 1 && faunes.size() > 3)
			throw std::invalid_argument("Une tuile doit avoir entre 1 et 3 faunes.");
	}

	///TODO ? un constructeur specifique pour extraire JSON?
	// constructeur par copie
	Tuile(const Tuile& other)
		: habitats(other.habitats),
		faunes(other.faunes),
		donneJetonNature(other.donneJetonNature),
		faunePlace(other.faunePlace),
		position(other.position ? std::make_unique<Position>(*other.position) : nullptr),
		placementConfirme(other.placementConfirme) {
	}
	// affectation par la surcharge de l'operateur
	Tuile& operator=(const Tuile& other) {
		if (this != &other) { 
			habitats = other.habitats;
			faunes = other.faunes;
			donneJetonNature = other.donneJetonNature;
			faunePlace = other.faunePlace;
			position = other.position ? std::make_unique<Position>(*other.position) : nullptr;
			placementConfirme = other.placementConfirme;
		}
		return *this;
	}
	// constructeur par defaut (Debug)
	Tuile()
		: habitats{}, faunes{}, donneJetonNature(false),
		faunePlace(std::nullopt), position(nullptr),
		placementConfirme(false) {
	}

	const std::array<Habitat, 6>& getHabitats() const { return habitats; }
	const std::vector<Faune>& getFaunes() const { return faunes; }
	bool getDonneJetonNature() const { return donneJetonNature; }
	bool JetonFaunePresent() const { return faunePlace.has_value(); }
	Faune getFaunePlace() const { return faunePlace.value_or(Faune::rien); }
	const Position& getPosition() const { return *(position.get()); }
	bool getPlacementConfirme() const { return placementConfirme; }


	void setPosition(int q, int r, int s) {
		if (!placementConfirme)
			position = std::make_unique<Position>(q, r, s);
	}

	void placerJetonFaune(Faune faune);

	void confirmerPlacement() { placementConfirme = true; }

	void pivoterHoraire();

	void pivoterAntiHoraire();

};

std::ostream& operator<<(std::ostream& flux, const Tuile& tuile);

void testClasseTuile();