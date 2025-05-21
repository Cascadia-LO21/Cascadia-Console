#pragma once
#include <optional>
#include <utility>
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
	std::array<std::optional<std::pair<std::optional<Tuile>, std::optional<JetonFaune>>>, 4> pioche;
	// a titre indicatif de visibilite des pieces dans la pioche pour QT
	std::array<std::pair<bool, bool>, 4> visibilite;
public:
	// constructeur = initiation des attributs
	Pioche() : visibilite{ {{false, false}, {false, false}, {false, false}, {false, false}} },
		pioche{} // Default initialization for pioche
	{
		for (size_t i = 0; i < pioche.size(); ++i) {
			pioche[i] = std::make_pair(Tuile(), JetonFaune()); // Assuming default constructors exist
		}
	}
	// setPaire(ancienneTuile, nouveauJeton) <- getPaire
	void resetAllJetonFaune(std::stack<Tuile>& pileTuiles, std::vector<JetonFaune>& sachetJetons);
	void removeAllJetonFaune();

	// reset les Paires dont l'indice est dans quiEnleverIndices
	void resetJetonFaune(std::stack<Tuile>& pileTuiles, std::vector<JetonFaune>& sachetJetons, const std::vector<int>& quiEnleverIndices);
	void removeJetonFaune(const std::vector<int>& quiEnleverIndices);
	// alternative de resetJetonFaune (son complement)
	void remplacerJetons(int except = -1);
	// Dans le cas de l'utilisation du jeton nature
	void removePair(size_t indiceTuile, size_t indiceJetonFaune);
	// simulation d'un joueur IA qui par defaut retire la derniere paire dans la pioche
	void removeLastPair();

	// Dans le cas usuel
	void retirerPaire(int i);
	// Update visibilite
	void retirerTuileVisible(unsigned int indexTuile);
	// Update visibilite
	void retirerJetonVisible(unsigned int indexJeton);
	// setter Paire
	void setPaire(unsigned int indice, const std::optional<Tuile>& tuile, const std::optional<JetonFaune>& jeton) {
		if (indice < 4) {
			pioche[indice] = std::make_pair(tuile, jeton);
			return;
		}
		throw std::out_of_range("Indice hors intervalle de la taille de la pioche");
	}
	// accesseur Paire
	std::pair<std::optional<Tuile>, std::optional<JetonFaune>> getPaire(int indice) const {
		if (indice < 0 || indice >= pioche.size()) {
			throw std::out_of_range("Indice hors intervalle de la taille de la pioche");
		}

		if (pioche[indice]) { // Vérifie si l'option a une valeur
			return *pioche[indice]; // Déréférence pour obtenir la paire
		}

		throw std::logic_error("Aucune paire disponible à cet indice");
	}
	const std::array<std::optional<std::pair<std::optional<Tuile>, std::optional<JetonFaune>>>, 4>& getPioche() const {
		return pioche;
	}

	// Verifications
	bool jetonsIdentiques(int nb) const;
	int retourneIndiceFaune(Faune type) const;
	bool quatreJetonsIdentiques() const;
	bool troisJetonsIdentiques() const;

	// Simulation du fonctionnement de piles dans la pioche apres choix de paire
	void slideApresJetonNature(int t, int j);
	// remplir pioche selon visibilite
	void rafraichirPioche(std::stack<Tuile>& pile, std::vector<JetonFaune>& jetons);
};

std::ostream& operator<<(std::ostream& os, const Pioche& p);