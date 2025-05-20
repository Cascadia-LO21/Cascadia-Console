#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <stdexcept>
#include "pioche.h"


// les methodes de la classe Pioche
void Pioche::resetAllJetonFaune() {

}

void Pioche::resetJetonFaune(const std::vector<int>& quiEnleverIndices) {

}

void Pioche::removePair(size_t indiceTuile, size_t indiceJetonFaune) {

}

void Pioche::removeLastPair() {
	// retirer indice = 3
	// update Visibilite
}


void Pioche::retirerPaire(int i) {

}

void Pioche::retirerTuileVisible(int indexTuile)
{
}

void Pioche::retirerJetonVisible(int indexJeton)
{
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

void Pioche::rafraichirPioche() {

}

std::ostream& operator<<(std::ostream& os, const Pioche& p) {

}
