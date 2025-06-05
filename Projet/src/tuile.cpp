#include <iostream>
#include <vector>
#include <array>
#include <optional>
#include <stdexcept>
#include <memory>
#include "tuile.h"
#include "affichage.h"


void Tuile::placerJetonFaune(Faune faune) {
	if (!JetonFaunePresent() && find(faunes.begin(), faunes.end(), faune) != faunes.end())
		faunePlace = faune;
	else
		throw std::invalid_argument("!La faune ne peut pas être placée ici!\n");
}

void Tuile::pivoterHoraire() {
	rotate(habitats.rbegin(), habitats.rbegin() + 1, habitats.rend());
}

void Tuile::pivoterAntiHoraire() {
	rotate(habitats.begin(), habitats.begin() + 1, habitats.end());
}

bool Tuile::operator==(const Tuile& autre) const
{
	return habitats == autre.getHabitats() && faunes == autre.getFaunes() && donneJetonNature == autre.getDonneJetonNature()
		&& faunePlace == autre.getFaunePlace() && *position == autre.getPosition() && placementConfirme == autre.getPlacementConfirme();
}


void testClasseTuile() {
	std::array<Habitat, 6> hab = { Habitat::fleuve, Habitat::fleuve, Habitat::fleuve,
								   Habitat::foret, Habitat::foret, Habitat::foret };
	std::vector<Faune> fau = { Faune::buse, Faune::renard, Faune::ours };

	std::unique_ptr<Position> p = std::make_unique<Position>(0, 1, -1);
	Tuile t(hab, fau, false, std::move(p));

	try {
		t.placerJetonFaune(Faune::buse);
	}
	catch (const char* e) {
		std::cout << e;
	}

	//t.setPosition(0, 1, -1);
	t.confirmerPlacement();
	t.pivoterHoraire();
	t.pivoterAntiHoraire();

	std::cout << t;
}
