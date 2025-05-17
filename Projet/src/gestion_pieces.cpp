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


std::vector<Tuile> GestionPieces::instancierTuiles(const std::string& fichier) {
	std::ifstream f(fichier);
	if (!f) throw std::runtime_error("Impossible d'ouvrir le fichier JSON : " + fichier);

	std::vector <Tuile> tuiles;

	json data = json::parse(f);

	for (const auto& tuile : data["tuiles"]) {

		// Conversion des habitats
		std::array<Habitat, 6> habitats;
		size_t i = 0;
		for (auto habitat : tuile["habitats"]) {
			habitats[i] = stringToHabitat(habitat.get<std::string>());
			i++;
		}

		if (i != 6 || i > habitats.size())
			throw std::runtime_error("Une tuile doit possède exactement 6 habitats !");

		// Conversion des faunes
		std::vector<Faune> faunes;
		for (auto faune : tuile["faunes"]) {
			faunes.push_back(stringToFaune(faune.get<std::string>()));
		}

		if (faunes.size() < 1 || faunes.size() > Tuile::maxFaunes)
			throw std::runtime_error("Le nombre de faunes qu'une tuile habrite va 1 a " + std::to_string(Tuile::maxFaunes));

		bool donneJetonNature = tuile.value("donneJetonNature", false); //false, valeur par défaut

		// Construction d'une Tuile directement dans le conteneur tuiles
		tuiles.emplace_back(habitats, faunes, donneJetonNature);
	}

	return tuiles;
}


///TODO : revoir ça pour être conforme à nlohmann::json

//void GestionPieces::instancierTuilesDepart(const string& fichier, vector<vector<Tuile>>& ensembleTuilesDepart) {
//	ifstream file(fichier);
//	if (!file.is_open()) {
//		throw runtime_error("Impossible d'ouvrir le fichier JSON.");
//	}
//
//	Json::Value root;
//	file >> root;
//
//	// Check if the root has the expected structure
//	if (!root.isMember("tuiles_depart") || !root["tuiles_depart"].isArray()) {
//		throw runtime_error("Format JSON incorrect : 'tuiles_depart' manquant ou non un tableau.");
//	}
//
//	size_t j = 0;
//	for (const auto& triplet : root["tuiles_depart"]) {
//		if (!triplet.isMember("triplet") || !triplet["triplet"].isArray()) {
//			throw runtime_error("Format JSON incorrect : 'triplet' manquant ou non un tableau.");
//		}
//
//		for (const auto& tuile : triplet["triplet"]) {
//			if (!tuile.isMember("tuile") || !tuile["tuile"].isObject()) {
//				throw runtime_error("Format JSON incorrect : 'tuile' manquant ou non un objet.");
//			}
//
//			const auto& donnee = tuile["tuile"];
//
//			array<Habitat, 6> habitats;
//			size_t i = 0;
//			if (donnee.isMember("habitats") && donnee["habitats"].isArray()) {
//				for (const auto& habitat : donnee["habitats"]) {
//					if (i < habitats.size()) {
//						habitats[i] = stringToHabitat(habitat.asString());
//						i++;
//					}
//					else {
//						throw runtime_error("Trop d'habitats dans 'tuile'.");
//					}
//				}
//			}
//			else {
//				throw runtime_error("Format JSON incorrect : 'habitats' manquant ou non un tableau.");
//			}
//
//			vector<Faune> faunes;
//			if (donnee.isMember("faunes") && donnee["faunes"].isArray()) {
//				for (const auto& faune : donnee["faunes"]) {
//					faunes.push_back(stringToFaune(faune.asString()));
//				}
//			}
//			else {
//				throw runtime_error("Format JSON incorrect : 'faunes' manquant ou non un tableau.");
//			}
//
//			bool donneJetonNature = donnee.get("donneJetonNature", false).asBool();
//
//			// allocation de memoire dynamique
//			if (j >= ensembleTuilesDepart.size()) {
//				ensembleTuilesDepart.resize(j + 1);
//			}
//
//			ensembleTuilesDepart[j].push_back(Tuile(habitats, faunes, donneJetonNature));
//		}
//		j++;
//	}
//}

void GestionPieces::melangerTuiles(std::vector<Tuile>& tuiles) {
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(tuiles.begin(), tuiles.end(), g);
}

///TODO: reflechir à implmenter en stack au lieu de vector
Tuile GestionPieces::depilerTuile(std::vector<Tuile>& tuiles) {
	if (tuiles.empty()) {
		throw std::out_of_range("Impossible de dépiler : vecteur vide !");
	}

	Tuile tuile = std::move(tuiles.back());
	tuiles.pop_back();
	return tuile;
}


void testGestionTuiles() {
	try {
		/*vector<Tuile> ensembleTuiles;
		GestionPieces::instancierTuiles("tuiles_non_reperes.json", ensembleTuiles);
		GestionPieces::instancierTuiles("tuiles_reperes.json", ensembleTuiles);*/

		//vector<vector<Tuile>> ensembleTuilesDepart;
		//GestionPieces::instancierTuilesDepart("tuiles_depart.json", ensembleTuilesDepart);

		//for (size_t i = 0; i < ensembleTuilesDepart.size(); ++i) {
		//	cout << "Triplet " << i + 1 << ":\n";
		//	for (const auto& tuile : ensembleTuilesDepart[i]) {
		//		cout << tuile << endl;  // Now this works
		//	}
		//	cout << "----------------------\n";
		//}

		//// Toutes les tuiles (Avant)
		//cout << "Ensemble de Tuiles avant de melanger et depiler:" << endl;
		//for (const auto& tuile : ensembleTuiles) {
		//    cout << tuile << endl;
		//}

		//// Toutes les tuiles melangees (Avant)
		//GestionPieces::melangerTuiles(ensembleTuiles);
		//cout << "Ensemble de Tuiles apres avoir ete melange sans depiler:" << endl;
		//for (const auto& tuile : ensembleTuiles) {
		//    cout << tuile << endl;
		//}

		////depiler la derniere Tuile
		//Tuile tuileDepilee = GestionPieces::depilerTuile(ensembleTuiles);
		//cout << "Tuile depilee: " << tuileDepilee << endl;

		//// le reste des tuiles (Apres)
		//cout << "Ensemble de Tuiles apres avoir ete melange et depile:" << endl;
		//for (const auto& tuile : ensembleTuiles) {
		//    cout << tuile << endl;
		//}
	}
	catch (const std::runtime_error& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Unexpected error: " << e.what() << std::endl;
	}
}

void testGestionTuiles2() {
	std::vector<Tuile> tuilesNonReperes = GestionPieces::instancierTuiles("tuiles_non_reperes.json");
	//for (const auto& tuile : tuilesNonReperes) cout << tuile << endl;

	std::vector<Tuile> tuilesReperes = GestionPieces::instancierTuiles("tuiles_reperes.json");
	//for (const auto& tuile : tuilesReperes) cout << tuile << endl;

	///TODO : peut etre qu'il faut fusionner les reperes et nonReperes

	GestionPieces::melangerTuiles(tuilesNonReperes);
	std::cout << GestionPieces::depilerTuile(tuilesNonReperes);

}