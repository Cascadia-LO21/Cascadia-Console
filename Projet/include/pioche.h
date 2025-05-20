#pragma once

#include <vector>
#include <array>
#include <stack>
#include <stdexcept>
#include <ostream>
#include "position.h"
#include "tuile.h"
#include "jeton_faune.h"
#include "enums.h"


// Cette classe implémente la logique de la pioche, gérant à la fois la pioche visible,
// à savoir, les 4 paires présentées au joueurs, ainsi que la pioche cachée, que sont
// que sont les tuiles restantes et les jetons restants.

class Pioche {
private:
	std::array<std::pair<Tuile, JetonFaune>, 4> pioche;
	// Privation du constructeur
public:

	// setterPair à indice specifique
	void setPair(size_t indice, const Tuile& tuile, const JetonFaune& jeton) const{}

	// getterPair à indice specifique
	std::pair<Tuile, JetonFaune> getPair(size_t indice) const {}

	// verification si pioche contient au moins un type de JetonFaune de cardinalité == nombre fois
	// fauneCount les faunes indice dans l'ordre de Enum Class
	bool contientJetonIdentique(int nombre) const;

	int retourneIndiceFaune(Faune type) const;

	bool estTroisIdentique() const;

	bool estQuatreIdentique() const;

	void retirerPaire(size_t indice);

	Tuile& nouvelleTuile();
	JetonFaune& nouveauJetonFaune();



	/// FONCTIONS DANS LE DOC SUITE A LA REU_4, à harmoniser avec celles deja deinies ci-haut ///
	void resetAllJetonFaune();
	void resetJetonFaune(const std::vector<int>& quiEnleverIndices);
	void removePair(size_t indiceTuile, size_t indiceJetonFaune);
	void removeLastPair();


	void retirerPaire(int i);
	void retirerTuileVisible(int indexTuile, int j);
	void retirerJetonVisible(int i, int j);
	void setPaire(int i, const Tuile& tuile, const JetonFaune& jeton);
	std::pair<Tuile, JetonFaune> getPaire(int i) const;

	bool jetonsIdentiques(int i) const;
	bool quatreJetonsIdentiques() const;
	bool troisJetonsIdentiques() const;

	void slideApresJetonNature(int t, int j);
	void remplacerJetons(int except = -1);
	void rafraichirPioche();

};

std::ostream& operator<<(std::ostream& os, const Pioche& p);