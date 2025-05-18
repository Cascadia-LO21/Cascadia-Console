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

std::vector<std::vector<Tuile>> GestionPieces::instancierTuilesDepart(const std::string& fichier) {
    std::ifstream file(fichier);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier JSON.");
    }

    json root;
    file >> root;

    // Check if the root has the expected structure
    if (!root.contains("tuiles_depart") || !root["tuiles_depart"].is_array()) {
        throw std::runtime_error("Format JSON incorrect : 'tuiles_depart' manquant ou non un tableau.");
    }

    std::vector<std::vector<Tuile>> ensembleTripletsDepart;

    for (const auto& triplet : root["tuiles_depart"]) {
        if (!triplet.contains("triplet") || !triplet["triplet"].is_array()) {
            throw std::runtime_error("Format JSON incorrect : 'triplet' manquant ou non un tableau.");
        }

        std::vector<Tuile> tuiles;

        for (const auto& tuile : triplet["triplet"]) {
            if (!tuile.contains("tuile") || !tuile["tuile"].is_object()) {
                throw std::runtime_error("Format JSON incorrect : 'tuile' manquant ou non un objet.");
            }

            const auto& donnee = tuile["tuile"];

            std::array<Habitat, 6> habitats;
            size_t i = 0;
            if (donnee.contains("habitats") && donnee["habitats"].is_array()) {
                for (const auto& habitat : donnee["habitats"]) {
                    if (i < habitats.size()) {
                        habitats[i] = stringToHabitat(habitat.get<std::string>());
                        i++;
                    }
                    else {
                        throw std::runtime_error("Trop d'habitats dans 'tuile'.");
                    }
                }
            }
            else {
                throw std::runtime_error("Format JSON incorrect : 'habitats' manquant ou non un tableau.");
            }

            std::vector<Faune> faunes;
            if (donnee.contains("faunes") && donnee["faunes"].is_array()) {
                for (const auto& faune : donnee["faunes"]) {
                    faunes.push_back(stringToFaune(faune.get<std::string>()));
                }
            }
            else {
                throw std::runtime_error("Format JSON incorrect : 'faunes' manquant ou non un tableau.");
            }

            bool donneJetonNature = donnee.value("donneJetonNature", false);

            // Directly construct Tuile with parameters
            tuiles.emplace_back(habitats, faunes, donneJetonNature);
        }

        // Add the filled vector of Tuiles to ensembleTripletsDepart
        ensembleTripletsDepart.push_back(std::move(tuiles)); // Move to avoid copying
    }

    return ensembleTripletsDepart;
}

std::vector<JetonFaune> GestionPieces::instancierJetonsFaunes()
{
    std::vector<JetonFaune> jetonsFaune;

    // 
    std::vector<Faune> FauneTypes = { Faune::saumon, Faune::ours, Faune::buse, Faune::renard, Faune::wapiti };

    // Instancier 20 jetons faune pour chaque type faune
    for (const auto& type : FauneTypes) {
        for (int i = 0; i < 20; ++i) {
            jetonsFaune.emplace_back(JetonFaune(type));
        }
    }

    return jetonsFaune;
}

template<typename T>
void GestionPieces::melanger(std::vector<T>& items)
{
    std::random_device rd;
    std::mt19937 eng(rd());
    std::shuffle(items.begin(), items.end(), eng);
}

void GestionPieces::melangerTuiles(std::vector<Tuile>& tuiles)
{
    melanger(tuiles);
}

void GestionPieces::melangerJetons(std::vector<JetonFaune>& jetons)
{
    melanger(jetons);
}

void GestionPieces::melangerTuilesDepart(std::vector<std::vector<Tuile>>& tuiles)
{
    melanger(tuiles); // melanger uniquement la couche exterieure des vecteurs
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
        //std::vector<Tuile> tuiles = GestionPieces::instancierTuiles("json/tuiles_non_reperes.json");

		// Afficher chaque tuile
		for (const auto& tuile : tuiles) {
			std::cout << tuile << std::endl; // Assurez-vous que l'opérateur << est surchargé pour Tuile
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Erreur : " << e.what() << std::endl;
	}


    try {
        // Instancier les tuiles à partir du fichier JSON
        std::vector<std::vector<Tuile>> triplets = GestionPieces::instancierTuilesDepart("json/tuiles_depart.json");

        // Afficher chaque triplet de tuiles
        for (const auto& tuiles : triplets) {
            for (const auto& tuile : tuiles) {
                std::cout << tuile << std::endl; // Assurez-vous que l'opérateur << est surchargé pour Tuile
            }
            std::cout << "-----" << std::endl; // Séparateur pour chaque triplet
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }
}
