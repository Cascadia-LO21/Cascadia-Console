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
	std::ifstream f(fichier);
	if (!f) throw std::runtime_error("Impossible d'ouvrir le fichier JSON : " + fichier);

	std::vector <Tuile> tuiles;

	json donneeJson = json::parse(f);

	for (const auto& tuile : donneeJson["tuiles"]) {

		// Extraction des habitats
		std::array<Habitat, 6> habitats;
		size_t i = 0;
		for (auto habitat : tuile["habitats"]) {
			habitats[i] = stringToHabitat(habitat.get<std::string>());
			i++;
		}

		if (i != 6 || i > habitats.size())
			throw std::runtime_error("Une tuile doit possède exactement 6 habitats !");

		// Extraction des faunes
		std::vector<Faune> faunes;
		for (auto faune : tuile["faunes"]) {
			faunes.push_back(stringToFaune(faune.get<std::string>()));
		}

		bool donneJetonNature = tuile.value("donneJetonNature", false);

		// Construction d'une Tuile directement dans le conteneur tuiles
		tuiles.emplace_back(habitats, faunes, donneJetonNature);
	}

	return tuiles;

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
void testGestionTuiles()
{
	try {
		// Instancier les tuiles à partir du fichier JSON
		std::vector<Tuile> tuiles = GestionPieces::instancierTuiles("json/tuiles_reperes.json");

		// Afficher chaque tuile
		for (const auto& tuile : tuiles) {
			std::cout << tuile << std::endl; // Assurez-vous que l'opérateur << est surchargé pour Tuile
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Erreur : " << e.what() << std::endl;
	}
}
