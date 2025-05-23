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
	std::unique_ptr<Position> position; // relation de composition
	bool placementConfirme;

public:
	// Constructeur principal
	Tuile(
		const std::array<Habitat, 6>& habitats,
		std::vector<Faune> faunes, // prend par valeur
		bool nature = false,
		std::unique_ptr<Position> position = nullptr,
		bool place = false)
		: habitats(habitats),
		faunes(std::move(faunes)), // deplace pour eviter de copier, on "vide" le parametre passé
		donneJetonNature(nature),
		faunePlace(std::nullopt),
		position(std::move(position)),
		placementConfirme(place) {
		if (this->faunes.size() < 1 || this->faunes.size() > 3)
			throw std::invalid_argument("Nombre de faunes invalide.");
	}


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

	// Debug only: permet de creer des Tuiles "vides" si on veut reserver x places dans un conteneur, comme vector<Tuile> tab(3)
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
std::ostream& operator<<(std::ostream& flux, const std::vector<Tuile>& tuileDepart);


void testClasseTuile();
