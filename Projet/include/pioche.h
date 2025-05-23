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
// les tuiles restantes et les jetons restants.

class Pioche {
private:
	static constexpr int MAX = 4; // taille par défaut officielle du nombre de couples (Tuile,JetonFaune) visible

	// Recense les 4 couples (Tuile,JetonFaune) visibles pour les joueurs.
	std::array<std::pair<Tuile, JetonFaune>, MAX> piocheVisible;

	// Voici un tableau qui decrit la presence de Tuile ou Jeton au sein de la pioche visible
	// false = invisible ; true = visible
	//std::array<std::pair<bool, bool>, 4> visibilite;
	
	// Permet de "cacher" temporairement une Tuile qui est posée sur le plateau, 
	// mais dont le JetonFaune associé n'est pas encore placé.
	std::array<bool, 4> tuilesVisibles; 

	// Les stocks suivants de Tuile et de JetonFaune forment la pioche cachée, 
	// dans laquelle on piochera des nouveaux elements.
	std::stack<Tuile> tuilesDispo;
	std::vector<JetonFaune> jetonsDispo;
	std::vector<std::vector<Tuile>> tuilesDepartDispo;

public:

	Pioche(unsigned int nbJoueurs = 1) :
		tuilesVisibles{ true }, // par defaut, tout est visible dans piocheVisible
		tuilesDispo{},
		jetonsDispo{},
		tuilesDepartDispo{}
	{
		setTuilesDispo(nbJoueurs);
		setJetonsDispo();
		setTuilesDepartDispo();
		setPiocheVisible();
	}


	/// SETTERS ///

	void setTuilesDispo(unsigned int nbJoueurs = 1);
	void setJetonsDispo();
	void setTuilesDepartDispo();
	void setTuilesVisibles(unsigned int i, bool vue);

	// Definit un couple (Tuile,JetonFaune) à la case [indice] de piocheVisible
	void setPaire(unsigned int indice, const Tuile& tuile, const JetonFaune& jeton) {
		if (indice >= MAX) {
			throw std::out_of_range("Indice " + std::to_string(indice) +
				" hors intervalle de la taille de la pioche (max = " +
				std::to_string(MAX) + ")");
		}
		piocheVisible[indice] = std::make_pair(tuile, jeton); 
	}

	void setPiocheVisible();



	/// GETTERS ///

	// type const auto& en retour, car autorise la lecture seulement
	const std::pair<Tuile,JetonFaune>& getPaire(unsigned int indice) const {
		if (indice >= MAX) {
			throw std::out_of_range("Indice " + std::to_string(indice) +
				" hors intervalle de la taille de la pioche (max = " +
				std::to_string(MAX) + ")");
		}

		return piocheVisible.at(indice);
	}

	//acces en lecture de la pioche visible
	const std::array<std::pair<Tuile, JetonFaune>, MAX>& getPiocheVisible() const {
		return piocheVisible;
	}

	const std::array<bool, 4>& getTuilesVisibles() const { return tuilesVisibles; }

	// donne l'info sur le nombre de pieces restantes, mais ne va pas reveler ce qu'il reste, puisque c'est une pioche cachée
	int getNbTuilesDispo() const {
		if (!tuilesDispo.empty()) return tuilesDispo.size();
	}

	int getNbJetonsDispo() const {
		if (!jetonsDispo.empty()) return jetonsDispo.size();
	}

	// methode qui sera appelée par Partie, pour distribuer ces TuilesDepart a chaque EnvJoueur
	const std::vector<std::vector<Tuile>>& getTuilesDepartDispo() { return tuilesDepartDispo; }



	/// AUTRES METHODES ///

	void resetAllJetonFaune(std::stack<Tuile>& pileTuiles, std::vector<JetonFaune>& sachetJetons);
	void removeAllJetonFaune();

	// reset les Paires dont l'indice est dans quiEnleverIndices
	void resetJetonFaune(std::stack<Tuile>& pileTuiles, std::vector<JetonFaune>& sachetJetons, const std::vector<int>& quiEnleverIndices);
	void removeJetonFaune(const std::vector<int>& quiEnleverIndices);
	// alternative de resetJetonFaune (son complement)
	void remplacerJetons(std::stack<Tuile>& pileTuiles, std::vector<JetonFaune>& sachetJetons, int except = -1);
	// Dans le cas de l'utilisation du jeton nature
	void removePair(unsigned int indiceTuile, unsigned int indiceJetonFaune);
	// simulation d'un joueur IA qui par defaut retire la derniere paire dans la pioche
	void removeLastPair();

	// Dans le cas usuel
	void retirerPaire(unsigned int i);
	// Update visibilite
	void retirerTuileVisible(unsigned int indexTuile);
	// Update visibilite
	void retirerJetonVisible(unsigned int indexJeton);
	
	// Verifications
	bool jetonsIdentiques(int nb) const;
	int retourneIndiceFaune(Faune type) const; //WARNING: definir dans le enum. Faire des static_cast
	bool quatreJetonsIdentiques() const;
	bool troisJetonsIdentiques() const;

	// Simulation du fonctionnement de piles dans la pioche apres choix de paire
	void slideApresJetonNature(int i, bool isTuile);
	void slideTuile(int i);
	void slideJeton(int i);

	// remplir pioche selon visibilite
	void rafraichirPioche(std::stack<Tuile>& pile, std::vector<JetonFaune>& jetons);
};

std::ostream& operator<<(std::ostream& os, const Pioche& p);