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
	// a titre indicatif de visibilite des pieces dans la pioche pour QT
	std::array<std::pair<int, int>, 4> visibilite;
public:
	// constructeur = initiation des attributs

	// setPaire(ancienneTuile, nouveauJeton) <- getPaire
	void resetAllJetonFaune();

	// reset les Paires dont l'indice est dans quiEnleverIndices
	void resetJetonFaune(const std::vector<int>& quiEnleverIndices);

	// alternative de resetJetonFaune (son complement)
	void remplacerJetons(int except = -1);
	// Dans le cas de l'utilisation du jeton nature
	void removePair(size_t indiceTuile, size_t indiceJetonFaune);
	// simulation d'un joueur IA qui par defaut retire la derniere paire dans la pioche
	void removeLastPair();

	// Dans le cas usuel
	void retirerPaire(int i);
	// Update visibilite
	void retirerTuileVisible(int indexTuile);
	// Update visibilite
	void retirerJetonVisible(int indexJeton);
	// setter Paire
	void setPaire(int i, const Tuile& tuile, const JetonFaune& jeton);
	// accesseur Paire
	std::pair<Tuile, JetonFaune> getPaire(int i) const;

	// Verifications
	bool jetonsIdentiques(int i) const;
	bool quatreJetonsIdentiques() const;
	bool troisJetonsIdentiques() const;

	// Simulation du fonctionnement de piles dans la pioche apres choix de paire
	void slideApresJetonNature(int t, int j);
	// remplir pioche selon visibilite
	void rafraichirPioche();

};

std::ostream& operator<<(std::ostream& os, const Pioche& p);