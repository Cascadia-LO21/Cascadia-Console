#include <iostream>
#include <vector>
#include <array>
#include <optional>
#include <stdexcept>
#include <memory>
#include "tuile.h"


std::ostream& operator<<(std::ostream& flux, const Tuile& tuile) {
	const auto& habitats = tuile.getHabitats();
	const auto& faunes = tuile.getFaunes();

	flux << "TUILE : \n";

	flux << "\t- Habitats : ";
	for (Habitat h : habitats) {
		flux << habitatToString(h) << ", ";
	}
	flux << "\n";

	flux << "\t- Faunes : ";
	for (Faune f : faunes) {
		flux << fauneToString(f) << ", ";
	}
	flux << "\n";

	if (tuile.getPlacementConfirme())
		flux << "\t- Position : " << tuile.getPosition() << "\n";

	if (tuile.getDonneJetonNature())
		flux << "\t- Donne Jeton Nature.\n";

	if (tuile.JetonFaunePresent())
		flux << "\t- Faune placée : " << fauneToString(tuile.getFaunePlace()) << ".\n";

	return flux;
}

//ostream pour vecteur de tuile like in envjoueur
std::ostream& operator<<(std::ostream& os, const std::vector<Tuile>& tuiles) {
	if (tuiles.empty()) {
		os << "Aucune tuile à afficher\n";
		return os;
	}

	for (const auto& tuile : tuiles) {
		os << "  (" << tuile.getPosition().getQ() << "," << tuile.getPosition().getR() << "," << tuile.getPosition().getS() << "): ";
		if (!tuile.getPlacementConfirme()) {
			os << " [NON CONFIRME] ";
		}
		if (tuile.getDonneJetonNature()) {
			os << " [REPERE]";
		}
		os << "\n";
		if (tuile.JetonFaunePresent()) {
			os << "    Jeton Faune Place: " << fauneToString(tuile.getFaunePlace()) << "\n";
		}
		else {
			os << "    Possibilite de placer: ";
			for (const auto& faune : tuile.getFaunes()) {
				os << fauneToString(faune) << " ";
			}
			os << "\n";
		}
		os << "    Habitats: ";
		for (const auto& habitat : tuile.getHabitats()) {
			os << habitatToString(habitat) << " ";
		}
		os << "\n";
	}
}

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
