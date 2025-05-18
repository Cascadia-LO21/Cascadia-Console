#pragma once

#include <ostream>
#include <vector>
#include <array>
#include <stack>
#include <string>
#include <random>
#include <algorithm>
#include <stdexcept>
#include "tuile.h"
#include "jeton_faune.h"


namespace GestionPieces {


	// Instancie toutes les tuiles à partir d'un fichier JSON
	std::vector<Tuile> instancierTuiles(const std::string& fichier = "json/tuiles_non_reperes.json");

	// Instancie les tuiles de départ à partir d'un fichier JSON
	std::vector<std::vector<Tuile>> instancierTuilesDepart(const std::string& fichier = "json/tuiles_depart.json");

	// Instancie tous les jetons faune 
	std::vector<JetonFaune> instancierJetonsFaunes();

	template<typename T>
	void melanger(std::vector<T>& items);

	// Mélange in-place un vecteur de tuiles
	void melangerTuiles(std::vector<Tuile>& tuiles);

	// Mélange in-place un vecteur de jetons faune
	void melangerJetons(std::vector<JetonFaune>& jetons);

	// Mélange in-place un vecteur de tuiles de depart
	void melangerTuilesDepart(std::vector<std::vector<Tuile>>& tuiles);

	// Fusionne deux vecteurs de tuiles en un seul
	std::vector<Tuile> fusionnerVecteursTuiles(const std::vector<Tuile>& v1, const std::vector<Tuile>& v2);

	// Convertit un vecteur de tuiles en pile (stack)
	std::stack<Tuile> vectorToStack(const std::vector<Tuile>& tuiles);

	// Pioche une tuile du dessus de la pile (stack)
	Tuile piocherTuile(std::stack<Tuile>& pile);

	// Remet une tuile sur la pile
	void remettreTuile(std::stack<Tuile>& pile, const Tuile& tuile);

	// Pioche un jeton faune du "sac" (vector, en général on pioche à la fin)
	JetonFaune piocherJeton(std::vector<JetonFaune>& jetons);

	// Remet un jeton faune dans le "sac"
	void remettreJeton(std::vector<JetonFaune>& jetons, const JetonFaune& jeton);

	// Pioche un triplet de tuiles de départ (par exemple, retire le premier du vector)
	std::tuple<Tuile, Tuile, Tuile> piocherTuileDepart(std::vector<std::tuple<Tuile, Tuile, Tuile>>& tuilesDepart);
}

void testGestionTuiles();