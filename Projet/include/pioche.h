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
	// Permet de "cacher" temporairement une Tuile ou Jeton qui est posée sur le plateau, 
	std::array<std::pair<bool, bool>, MAX> visibilite;
	
	// Les stocks suivants de Tuile et de JetonFaune forment la pioche cachée, 
	// dans laquelle on piochera des nouveaux elements.
	std::stack<Tuile> tuilesDispo;
	std::vector<JetonFaune> jetonsDispo;
	std::vector<std::vector<Tuile>> tuilesDepartDispo;

	// Pour factoriser la verification d'indice ; methode pas necessaire de rendre disponible à tout utilisateur
	void checkIndex(unsigned int indice) const {
		if (indice >= MAX) {
			throw std::out_of_range("Indice " + std::to_string(indice) +
				" hors intervalle (max = " + std::to_string(MAX) + ")");
		}
	}

	/// SETTERS PRIVES ///
	void setTuilesDispo(unsigned int nbJoueurs = 1);
	void setJetonsDispo();
	void setTuilesDepartDispo();
	void setPiocheVisible();

public:

	Pioche(unsigned int nbJoueurs = 1) :
		visibilite{ {{true,true},{true,true},{true,true},{true,true} } }, // par defaut, tout est visible dans piocheVisible
		tuilesDispo{},
		jetonsDispo{},
		tuilesDepartDispo{}
	{
		setTuilesDispo(nbJoueurs);
		setJetonsDispo();
		setTuilesDepartDispo();
		setPiocheVisible();
	}


	/// SETTER PUBLIC ///

	// Definit un couple (Tuile,JetonFaune) à la case [indice] de piocheVisible
	void setPaire(unsigned int indice, const Tuile& tuile, const JetonFaune& jeton) {
		checkIndex(indice);
		piocheVisible[indice] = std::make_pair(tuile, jeton); 
	}

	void setVisibilite(unsigned int i, bool vue, bool tuile = true); // utile pour EnvJoueur quand un joueur veut annuler la tuile choisie


	/// GETTERS ///

	// type const auto& en retour, car autorise la lecture seulement
	const std::pair<Tuile,JetonFaune>& getPaire(unsigned int indice) const {
		checkIndex(indice);
		return piocheVisible.at(indice);
	}

	//acces en lecture de la pioche visible
	const std::array<std::pair<Tuile, JetonFaune>, MAX>& getPiocheVisible() const {
		return piocheVisible;
	}

	const std::array<std::pair<bool,bool>, MAX>& getVisibilite() const { return visibilite; }

	// donne l'info sur le nombre de pieces restantes, mais ne va pas reveler ce qu'il reste, puisque c'est une pioche cachée
	size_t getNbTuilesDispo() const {
		if (!tuilesDispo.empty()) return tuilesDispo.size();
	}

	size_t getNbJetonsDispo() const {
		if (!jetonsDispo.empty()) return jetonsDispo.size();
	}

	// methode qui sera appelée par Partie, pour distribuer ces TuilesDepart a chaque EnvJoueur
	std::vector<std::vector<Tuile>>& getTuilesDepartDispo() { return tuilesDepartDispo; }

	int getMax() const { return MAX; }



	/// AUTRES METHODES ///

	void resetAllJetonFaune();
	void resetJetonFaune(const std::vector<unsigned int>& indices);
	void resetTroisJetonsIdentiques();

	void retirerTuileVisible(unsigned int indexTuile);
	void retirerJetonVisible(unsigned int indexJeton, bool remettre = false);


	void retirerTuileJetonDebut(); // simulation d'un joueur robot qui par defaut retire la 1e tuile et 1e jeton visible dans la pioche
	void retirerPaire(unsigned int i); // retirer un couple fixe
	void retirerPaire(unsigned int i, unsigned int j); //retirer un couple libre par action Jeton Nature


	bool jetonsIdentiques(int nb) const;
	bool quatreJetonsIdentiques() const;
	bool troisJetonsIdentiques() const;


	// Simulation du fonctionnement (graphique) dans la pioche apres choix de paire
	void slide(unsigned int i = 0, bool tuile = false);


	void rafraichirPioche();
};

std::ostream& operator<<(std::ostream& os, const Pioche& p);

void testPioche(int nbJoueur);
void testPioche1joueur();
void testPiocheBcpJoeurs(int n);
void testPiocheJoueurUndo(int n);