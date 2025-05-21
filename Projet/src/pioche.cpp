#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <stdexcept>
#include "pioche.h"
#include "gestion_pieces.h"


// les methodes de la classe Pioche
void Pioche::resetAllJetonFaune() {
	
}

void Pioche::removeAllJetonFaune() {
	for (size_t i = 0; i < pioche.size(); ++i) {
		pioche[i]->second = std::nullopt;
		retirerJetonVisible(i);
	}
}

void Pioche::resetJetonFaune(const std::vector<int>& quiEnleverIndices) {

}

void Pioche::removeJetonFaune(const std::vector<int>& quiEnleverIndices) {
	for (int indice : quiEnleverIndices) {
		if (indice >= 0 && indice < pioche.size()) {
			if (pioche[indice]) {
				pioche[indice]->second = std::nullopt;
				retirerJetonVisible(indice);
			}
		}
		else {
			throw std::out_of_range("Indice hors intervalle pour retirer le jeton Faune");
		}
	}
}

void Pioche::removePair(size_t indiceTuile, size_t indiceJetonFaune) {
	// verification des parametres en entree
	if (indiceTuile >= pioche.size()) {
		throw std::out_of_range("Indice de tuile hors intervalle");
	}
	if (indiceJetonFaune >= pioche.size()) {
		throw std::out_of_range("Indice de jeton Faune hors intervalle");
	}

	// verification d'existences
	if (pioche[indiceTuile] && pioche[indiceJetonFaune]) {
		pioche[indiceTuile]->first = std::nullopt; // Retire la Tuile
		pioche[indiceTuile]->second = std::nullopt; // Retire le JetonFaune
		retirerTuileVisible(indiceTuile);
		retirerJetonVisible(indiceJetonFaune);
	}
	else {
		throw std::logic_error("Aucune paire disponible à l'indice de tuile donné");
	}
}

void Pioche::removeLastPair() {
	if (pioche[3]) {
		// retirer paire indice = 3
		pioche[3] = std::nullopt; // optional

		// MAJ visibilite
		retirerTuileVisible(3);
		retirerJetonVisible(3);
	}
	else {
		throw std::logic_error("Aucune paire à retirer à l'indice 3");
	}
}


void Pioche::retirerPaire(int indice) {
	if (indice < 0 || indice >= pioche.size()) {
		throw std::out_of_range("Indice hors intervalle pour retirer la paire");
	}

	// verif existence
	if (pioche[indice]) {
		pioche[indice] = std::nullopt;

		retirerTuileVisible(indice);
		retirerJetonVisible(indice);
	}
	else {
		throw std::logic_error("Aucune paire disponible à cet indice");
	}
}

void Pioche::retirerTuileVisible(unsigned int indexTuile)
{
	if (indexTuile < visibilite.size()) {
		visibilite[indexTuile].first = false;
	}
	else {
		throw std::out_of_range("Indice hors intervalle pour retirer la tuile visible");
	}
}

void Pioche::retirerJetonVisible(unsigned int indexJeton)
{
	if (indexJeton < visibilite.size()) {
		visibilite[indexJeton].second = false;
	}
	else {
		throw std::out_of_range("Indice hors intervalle pour retirer le jeton visible");
	}
}

bool Pioche::jetonsIdentiques(int nb) const {
	std::array<int, 5> fauneCount{ 0 };
	// compter respectivement le nb de chaque faune presente
	for (const auto& optionalPair : pioche) {
		if (optionalPair) { // Verification d'existence de Paire
			fauneCount[retourneIndiceFaune(optionalPair->second.getType())]++;
		}
	}
	// verification du nombre
	for (const auto& value : fauneCount) {
		if (value == nb) {
			return true;
		}
	}
	return false;
}

int Pioche::retourneIndiceFaune(Faune type) const
{
	switch (type) {
	case Faune::saumon: return 0;
	case Faune::ours:   return 1;
	case Faune::buse:   return 2;
	case Faune::renard: return 3;
	case Faune::wapiti: return 4;
	default: throw std::invalid_argument("Type de Faune Inconnu");
	}
}

bool Pioche::quatreJetonsIdentiques() const {
	return jetonsIdentiques(4);
}

bool Pioche::troisJetonsIdentiques() const {
	return jetonsIdentiques(3);
}

void Pioche::slideApresJetonNature(int t, int j) {

}

void Pioche::remplacerJetons(int except) {

}

void Pioche::rafraichirPioche(std::stack<Tuile>& pileTuiles, std::vector<JetonFaune>& sachetJetons) {
	for (size_t i = 0; i < pioche.size(); ++i) {
		auto [ancienTuile, ancienJeton] = getPaire(i);
		if (!visibilite[i].first) {
			if (!pileTuiles.empty()) {
				Tuile nouvelleTuile = GestionPieces::piocherTuile(pileTuiles);
				visibilite[i].first = true;
				setPaire(i, nouvelleTuile, ancienJeton);
			}
		}
		if (!visibilite[i].second) {
			if (!sachetJetons.empty()) {
				JetonFaune nouveauJeton = GestionPieces::piocherJeton(sachetJetons);
				visibilite[i].second = true;
				setPaire(i, ancienTuile, nouveauJeton);
			}
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Pioche& p) {
	os << "Pioche :\n";
	for (size_t i = 0; i < p.getPioche().size(); ++i) {
		os << "Index " << i << ": ";
		try {
			auto [tuile, jeton] = p.getPaire(i); // Utilisation de getPaire
			os << "Tuile : ";
			if (tuile) {
				os << *tuile; // Utilisation de l'opérateur << pour Tuile
			}
			else {
				os << "Aucune";
			}
			os << ", JetonFaune : ";
			if (jeton) {
				os << *jeton; // Utilisation de l'opérateur << pour JetonFaune
			}
			else {
				os << "Aucun";
			}
		}
		catch (const std::logic_error& e) {
			os << "Erreur : " << e.what(); // Gestion des erreurs
		}
		os << '\n'; // Utilisation de '\n' pour une meilleure performance
	}
	return os;
}