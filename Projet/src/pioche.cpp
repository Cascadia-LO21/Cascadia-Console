#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <stdexcept>
#include "pioche.h"


// les methodes de la classe Pioche
// les methodes de la classe Pioche
void Pioche::resetAllJetonFaune() {

}

void Pioche::resetJetonFaune(const std::vector<int>& quiEnleverIndices) {

}

void Pioche::removePair(size_t indiceTuile, size_t indiceJetonFaune) {

}

void Pioche::removeLastPair() {

}


void Pioche::retirerPaire(int i) {

}

void Pioche::retirerTuileVisible(int indexTuile)
{
}

void Pioche::retirerJetonVisible(int indexJeton)
{
}

void Pioche::setPaire(int i, const Tuile& tuile, const JetonFaune& jeton) {

}

std::pair<Tuile, JetonFaune> Pioche::getPaire(int i) const {

}


bool Pioche::jetonsIdentiques(int i) const {

}

bool Pioche::quatreJetonsIdentiques() const {

}

bool Pioche::troisJetonsIdentiques() const {

}

void Pioche::slideApresJetonNature(int t, int j) {

}

void Pioche::remplacerJetons(int except) {

}

void Pioche::rafraichirPioche() {

}

std::ostream& operator<<(std::ostream& os, const Pioche& p) {

}
