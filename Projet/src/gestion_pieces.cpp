#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <string>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include "gestion_pieces.h"
#include "enums.h"
#include "tuile.h"
#include "json.hpp"
using json = nlohmann::json;

std::vector<Tuile> GestionPieces::instancierTuiles(const std::string& fichier)
{
	return std::vector<Tuile>();
}

std::vector<std::tuple<Tuile, Tuile, Tuile>> GestionPieces::instancierTuilesDepart(const std::string& fichier)
{
	return std::vector<std::tuple<Tuile, Tuile, Tuile>>();
}

std::vector<JetonFaune> GestionPieces::instancierJetonsFaunes()
{
	return std::vector<JetonFaune>();
}

void GestionPieces::melangerTuiles(std::vector<Tuile>& tuiles)
{
}

void GestionPieces::melangerJetons(std::vector<JetonFaune>& jetons)
{
}

std::vector<Tuile> GestionPieces::fusionnerVecteursTuiles(const std::vector<Tuile>& v1, const std::vector<Tuile>& v2)
{
	return std::vector<Tuile>();
}

std::stack<Tuile> GestionPieces::vectorToStack(const std::vector<Tuile>& tuiles)
{
	return std::stack<Tuile>();
}

//Tuile GestionPieces::piocherTuile(std::stack<Tuile>& pile)
//{
//	return Tuile();
//}
//
//void GestionPieces::remettreTuile(std::stack<Tuile>& pile, const Tuile& tuile)
//{
//}
//
//JetonFaune GestionPieces::piocherJeton(std::vector<JetonFaune>& jetons)
//{
//	return JetonFaune();
//}
//
//void GestionPieces::remettreJeton(std::vector<JetonFaune>& jetons, const JetonFaune& jeton)
//{
//}
//
//std::tuple<Tuile, Tuile, Tuile> GestionPieces::piocherTuileDepart(std::vector<std::tuple<Tuile, Tuile, Tuile>>& tuilesDepart)
//{
//	return std::tuple<Tuile, Tuile, Tuile>();
//}
//
//void testGestionTuiles()
//{
//}
