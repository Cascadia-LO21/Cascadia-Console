//#include <iostream>
//#include <vector>
//#include <array>
//#include <stack>
//#include <stdexcept>
//#include "pioche.h"
//
//
//// les methodes de la classe Pioche
//// verification si pioche contient au moins un type de JetonFaune de cardinalité == count fois
//	// fauneCount les faunes indice dans l'ordre de Enum Class
//bool Pioche::contientJetonIdentique(int nombre) const {
//	std::array<int, 5> fauneCount{ 0 };
//	// compter respectivement le nb de chaque faune presente
//	for (const auto& pair : pioche) {
//		fauneCount[retourneIndiceFaune(pair.second.getType())]++;
//	}
//	// verification du nombre
//	for (const auto& value : fauneCount) {
//		if (value == nombre) {
//			return true;
//		}
//	}
//	return false;
//}
//
//int Pioche::retourneIndiceFaune(Faune type) const {
//	switch (type) {
//	case Faune::saumon: return 0;
//	case Faune::ours:   return 1;
//	case Faune::buse:   return 2;
//	case Faune::renard: return 3;
//	case Faune::wapiti: return 4;
//	default: throw std::invalid_argument("Type de Faune Inconnu");
//	}
//}
//
//bool Pioche::estTroisIdentique() const {
//	return contientJetonIdentique(3);
//}
//
//bool Pioche::estQuatreIdentique() const {
//	return contientJetonIdentique(4);
//}
//
//void Pioche::retirerPaire(size_t indice) {
//	if (indice < 4) {
//		pioche[indice] = std::make_pair(nouvelleTuile(), nouveauJetonFaune());
//	}
//	else {
//		throw std::out_of_range("Indice hors intervalle de la taille de la pioche");
//	}
//}
//
//void Pioche::resetAllJetonFaune() {
//	for (int i = 0; i < 4; i++) {
//		pioche[i].second = nouveauJetonFaune();
//		// sans faire faune type -- (t'as dis quoi nicolas?)
//	}
//}
//
//void Pioche::resetJetonFaune(const std::vector<int>& quiEnleverIndices) {
//	for (int i : quiEnleverIndices) {
//		if (i >= 0 && i < 4) {
//			pioche[i].second = nouveauJetonFaune();
//		}
//		else {
//			//throw std::out_of_range("Indice hors intervalle de la taille de la pioche, ignoré");
//			//^si on veut que le programme s'arrête là, sinon:
//			std::cerr << "Indice " << i << " hors intervalle. Ignoré.\n";
//		}
//	}
//}
//
//void Pioche::removePair(size_t indiceTuile, size_t indiceJetonFaune) {
//	if (indiceTuile >= 4 || indiceJetonFaune >= 4) {
//		throw std::out_of_range("Indice hors de la pioche");
//	}
//	//Décalage des jetons Faune vers la droite ( on comble le "trou"): on remets des index 1 à celui du jeton pris à leur place où ils 'tombent'
//	for (size_t i = indiceJetonFaune; i > 0; i--) {
//		pioche[i].second = pioche[i - 1].second;//!!!!!!!!!!!!!!!!!!!!est ce qu'on aurait besoin d'un constructeur par recopie de tuile et jeton?
//	}
//	//de même pour tuile
//	for (size_t i = indiceTuile; i > 0; i--) {
//		pioche[i].first = pioche[i - 1].first;
//	}
//	//on génère le nouveau jeton et tuile en tete de la pioche
//	pioche[0] = std::make_pair(nouvelleTuile(), nouveauJetonFaune());
//}
////^when you choose une pair (tuile et jetonFaune) en utilisant un jetonNature: they fall down and you get new pairs
//
//
//
//
//
//
////to be continued:
//void Pioche::removeLastPair() { //!!!!!!!!!!!!!!!!!!!!!!!!!à en parler. simulates fake player //i think this function should be done after choosing the architecture of how the game works
//	//if (pioche[3]) {//si la dernière est valide == le joueur n'a pas choisi cette paire
//	//	//enlever la dernière paire (indice 3)
//	//	pioche[3] = pioche[2];
//	//	pioche[2] = pioche[1];
//	//	pioche[1] = pioche[0];
//	//	pioche[0] = std::make_pair(nouvelleTuile(), nouveauJetonFaune());
//	//}
//	//else {
//	//	//enlever la paire à l'indice 2 (dernière paire valide)
//
//	//}
//}
