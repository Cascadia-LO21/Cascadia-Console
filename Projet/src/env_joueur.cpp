#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <stdexcept>
#include <random>
#include <algorithm>
#include "position.h"
#include "tuile.h"
#include "env_joueur.h"
#include "gestion_pieces.h" //debug only
#include "affichage.h"

bool EnvJoueur::aTuile(const Position& coord) const {
	return tuiles.find(coord) != tuiles.end(); //find trouve la tuile avec la clé coord (renvoie tuiles.end() sinon), end renvoie pointeur vers l'élément après le dernier de tuiles
};

bool EnvJoueur::aTuileConfirmee(const Position& coord) const {
	auto it = tuiles.find(coord);
	if (it != tuiles.end()) {
		return it->second.getPlacementConfirme(); //on accède à la tuile et on vérifie si son placement est confirmé
	}
	return false; //pas de tuile confirmée à cette position
};

//get pointeur de la tuile à cette coordonnée (retourne nullptr si pas de tuile à cette coordonnée)
const Tuile* EnvJoueur::getTuile(const Position& coord) const {
	auto it = tuiles.find(coord);//on trouve la paire <Position,Tuile>
	if (it != tuiles.end()) {//si elle existe, alors on retourne pointeur vers la tuile
		return &(it->second);
	}
	return nullptr;
}
//version non-const de getTuile, pour permettre la modification de la tuile
Tuile* EnvJoueur::getTuileNonConst(const Position& coord){
	auto it = tuiles.find(coord);//on trouve la paire <Position,Tuile>
	if (it != tuiles.end()) {//si elle existe, alors on retourne pointeur vers la tuile
		return &(it->second);
	}
	return nullptr;
}

//est ce qu'on peut placer une tuile à cette coordonnée?
bool EnvJoueur::positionTuileValide(const Position& coord) const {
	//hyp: une tuile pour être placé, doit être au moins adjacente à une autre tuile
	//est ce qu'il y a déjà une tuile dans cette position?
	if (tuiles.find(coord) != tuiles.end()) {
		//donc il y a déjà une tuile à cette endroit.
		return false;
	}
	//est ce qu'il y a au moins une tuile adjacente à cette position? si oui donc valide
	std::vector<Position> voisins = coord.getVecteurPositionsAdjacentes();
	for (const Position& v : voisins) {
		//std::cout << "Checking position: " << v << std::endl; // Debugging line
		if (aTuile(v)) {
			return true;
		}
	}
	//aucune tuile adjacente (aucun voisin)
	return false;
};

// on ne modifie pas encore l'attribut position dans la tuile, on ne le fera que quand cest confirme
// a ce stade, la tuile : position = nullptr ET placementConfirme = false
void EnvJoueur::placerTuile(const Position& coord, Tuile& tuile) {
	if (!positionTuileValide(coord)) {
		throw std::invalid_argument("Position invalide pour placer la tuile");
	}

	//si y'avait déjà une tentative de placement pour cette tuile, undo en premier
	if (placementEnAttente) {
		undoDernierPlacement();
	}

	//placer la tuile
	tuile.setPosition(coord); // la tuile a une position, mais non confirme pour l'instant
	tuiles[coord] = tuile;
	//tuiles[coord].setPosition(coord);

	//se rappeler de ce placement
	dernierePosition = coord; 
	derniereTuile = tuile;
	placementEnAttente = true; //on a un placement en attente de confirmation

};

void EnvJoueur::confirmerPlacement() {
	if (!placementEnAttente) {
		throw std::logic_error("Aucun placement en attente à confirmer");
	}
	if (!dernierePosition.has_value() || !derniereTuile.has_value()) {
		throw std::logic_error("Aucune tuile ou position en attente de confirmation");
	}

	// marquer dans la tuile que sa position est confirmée
	//tuiles[dernierePosition.value()].setPosition(dernierePosition.value()); // mettre a jour la position de la tuile en attente
	tuiles[dernierePosition.value()].confirmerPlacement();

	//std::cout << "\nDANS CONFIRME PLACEMENT, eske la pos encore là ? : " <<
		tuiles[dernierePosition.value()].positionDefinie();

	// maintenant, la Tuile a bien : une position definitive ET placementConfirme = true

	// reinitialiser les variables d'attente
	placementEnAttente = false;
	dernierePosition.reset();
	derniereTuile.reset(); 
};

bool EnvJoueur::undoDernierPlacement() {
	if (!placementEnAttente || !dernierePosition.has_value() || !derniereTuile.has_value()) {
		return false; //rien à annuler
	}

	//retirer la tuile de la map
	derniereTuile->setPositionNull(); // supprimer la derniere position non confirmee
	tuiles.erase(dernierePosition.value()); // rien a modifier dans la tuile, qui n'a toujours pas de position definie

	//vider les variables d'attente
	placementEnAttente = false;
	dernierePosition.reset(); //on n'a plus de position en attente
	derniereTuile.reset(); //on n'a plus de tuile en attente

	return true; //annulation réussie
}

void EnvJoueur::placerTuileDefinitive(const Position& coord, Tuile& tuile) {
	placerTuile(coord, tuile);
	confirmerPlacement();
}

//retourne -1 si le jeton ne peut être placé nulpart, 0 si le jeton ne peut etre placé à cette position, 1 si le jeton est placé avec succès
int EnvJoueur::placerJetonFaune(const Position& coord, const JetonFaune& jeton) {

	//étape 1: vérification primitive, est-ce qu'on peut placer le jeton sur une tuile quelconque dans la map?
	// (oui si il y a au moins une tuile qui a parmi son vecteur faunes le type du jeton passé en paramètre et
	// qui n'a pas de jeton déjà placé dessus)
	bool peutPlacerJeton = false;
	for (const auto& [pos, tuile] : tuiles) {
		if (!tuile.JetonFaunePresent()) { // est ce qu'il y a un jeton placé déjà sur cette tuile?
			const auto& faunes = tuile.getFaunes();
			if (std::any_of(faunes.begin(), faunes.end(),
				[&](const JetonFaune& j) { return j.getType() == jeton.getType(); })) {// Vérifie si faunes contient le type du jeton entrée
				peutPlacerJeton = true;
				break;
				//IL FAUDRA PERMETTRE A L'UTILISATEUR DE L'ENLEVER DE LA PIOCHE, ou de choisir une autre paire tuile/jeton
			}
		}
	}
	if (!peutPlacerJeton) {
		return -1; //jeton ne peut pas etre placé sur aucunes tuiles dans la map
	}

	//étape 2: est-ce qu'il y a une tuile confirmée libre à cette coordonnée?
	//if (!aTuileConfirmee(coord)) { return 0; }  // un jeton peut etre place sur une tuile qui vient d etre ajoute au plateau

	Tuile* tuilePtr = getTuileNonConst(coord);
	if (!tuilePtr) {
		throw std::logic_error("Aucune tuile à cette position");
	}
	if (tuilePtr->JetonFaunePresent()) {
		return 0; //jeton ne peut pas etre placé, il y a déjà un jeton sur cette tuile
	}

	//étape 3: si la tuile à cette coordonnée a parmis son vecteur faunes le type du jeton passé en paramètre
	bool typePresent = std::any_of(tuilePtr->getFaunes().begin(), tuilePtr->getFaunes().end(),
		[&](const JetonFaune& j) { return j.getType() == jeton.getType(); });
	if (!typePresent) {
		return 0; //jeton ne peut pas etre placé
	}

	//étape 4: on place le jeton sur la tuile à cette coordonnée
	tuilePtr->placerJetonFaune(jeton.getType());
	//on ajoute la position du jeton dans la map de positions des jetons
	mapPositionsJetons[jeton.getType()].insert(coord);
	if (tuilePtr->getDonneJetonNature()) {
		//si la tuile donne un jeton nature, on incrémente le nombre de jeton nature du joueur
		nbJetonNature++;
	}
	return 1; //jeton placé avec succès
}


//reset l'envJoueur et y ajoute les tuiles de départ
void EnvJoueur::setTuilesDepart(std::vector<Tuile>& tuilesDepart) {
	if (tuilesDepart.empty()) {
		throw std::invalid_argument("Le vecteur de tuiles de départ possibles est vide.");
	}

	// On vide les tuiles existantes
	tuiles.clear();

	if (tuilesDepart.size() < 3) {
		throw std::invalid_argument("Le vecteur de tuiles de départ n'a pas 3 tuiles.");
	}

	// Placer les 3 tuiles selon les positions définies
	// Ordre attendu : haut, gauche, droite(centre)
	placerTuileDefinitiveDepart(Position(0,-1,1), tuilesDepart[0]);
	placerTuileDefinitiveDepart(Position(-1, 0, 1), tuilesDepart[1]);
	placerTuileDefinitiveDepart(Position(0, 0, 0), tuilesDepart[2]);
}

// ICI position definie ok
void EnvJoueur::placerTuileDepart(const Position& coord, Tuile& tuile){

	//tuile.setPosition(coord); // update position avant de le mettre dans la map
	tuiles[coord] = tuile;
	tuiles[coord].setPosition(coord);
	std::cout << "\nJAI SET LA POSTION DE LA TUILE DEPART : ";
	std::cout << tuiles[coord].positionDefinie();

	dernierePosition = coord;
	derniereTuile = tuiles[coord];
	placementEnAttente = true;

}

//void EnvJoueur::placerTuileDefinitiveDepart(const Position& coord, Tuile& tuile){
//	placerTuileDepart(coord, tuile);
//	std::cout << "\n1. et mtn eske pos encore là? " << tuile.positionDefinie();
//
//	confirmerPlacement();
//	std::cout << "\n2. et mtn eske pos encore là? " << tuile.positionDefinie();
//
//
//	//tuiles[coord] = tuile;
//	//tuile.setPosition(coord);
//	//tuile.confirmerPlacement();
//}


void EnvJoueur::placerTuileDefinitiveDepart(const Position& coord, Tuile& tuile) {
	tuiles[coord] = tuile; // Copie la tuile dans la map 
	Tuile& tuileDansMap = tuiles[coord]; // Référence sur la tuile dans la map 
	tuileDansMap.setPosition(coord); // Met à jour la position de la tuile dans la map

	dernierePosition = coord;
	derniereTuile = tuileDansMap;
	placementEnAttente = true;

	//std::cout << "\n1. et mtn eske pos encore là? " << tuileDansMap.positionDefinie();

	confirmerPlacement(); // Confirme le placement de la tuile dans la map

	//std::cout << "\n2. et mtn eske pos encore là? " << tuileDansMap.positionDefinie();
} 


// retourne un vecteur de Tuile qui ont encore des cotes libres
std::vector<Tuile> EnvJoueur::getTuilesAvecVoisinLibre() const {
	std::vector<Tuile> res; 

	for (const auto& [position, tuile] : tuiles) {
		std::vector<Position> posVoisines = position.getVecteurPositionsAdjacentes();
		bool aVoisinLibre = false;
		for (const Position& voisin : posVoisines) {
			if (!aTuile(voisin)) {  // si la tuile a proximite n'est pas encore occupee par une Tuile
				aVoisinLibre = true; // alors la place est libre et peut accueillir une Tuile
				break; // Pas besoin de continuer si un voisin libre est trouvé
			}
		}
		if (aVoisinLibre && std::find(res.begin(), res.end(), tuile) == res.end()) { // la tuile n'est pas encore visite
			res.push_back(tuile); // tuile a des places libres autour d'elle
		}
	}
	return res;
}

std::vector<Direction> EnvJoueur::getDirLibresAutourTuile(const Tuile& tuile) const {
	const Position& tuilePos = tuile.getPosition();
	const std::vector<Position> posVoisines = tuilePos.getVecteurPositionsAdjacentes();
	
	std::vector<Direction> res;
	for (const Position& voisin : posVoisines) {
		if (!aTuile(voisin)) {
			Direction dirVersVoisin = tuilePos.getDirectionAdjacente(voisin);
			if (dirVersVoisin != Direction::Inconnue) { // Par sécurité
				res.push_back(dirVersVoisin);
			}
		}
	}
	return res;
}

std::vector<Position> EnvJoueur::getPosLibresAutourTuile(const Tuile& tuile) const{
	const Position& tuilePos = tuile.getPosition();
	const std::vector<Position> posVoisines = tuilePos.getVecteurPositionsAdjacentes();

	std::vector<Position> res;
	for (const Position& voisin : posVoisines) {
		if (!aTuile(voisin)) {
			res.push_back(voisin);
		}
	}
	return res;
}

// retourne toutes les positions de tuiles, telles qu'une tuile n'ait pas encore de Faune placee ET son vecteur de Faunes contient la faune
std::vector<Position> EnvJoueur::getPosLibresPourJeton(Faune f) const {
	std::vector<Position> posLibres;
	for (const auto& [position, tuile] : tuiles) {
		// Vérifie qu'il n'y a pas déjà de jeton faune sur la tuile
		if (!tuile.JetonFaunePresent()) {
			// Vérifie que la faune f est bien présente dans les faunes de la tuile
			const auto& faunes = tuile.getFaunes();
			if (std::find(faunes.begin(), faunes.end(), f) != faunes.end()) {
				posLibres.push_back(position);
			}
		}
	}
	return posLibres;
}



void testHexagonalDisplay() {
	std::cout << "=== Test Affichage Hexagonal ===\n\n";

	// Test 1: Empty environment
	std::cout << "Test 1: Environnement vide\n";
	EnvJoueur joueur1("Alice", 5);
	std::cout << joueur1 << "\n";
	std::cout << std::string(50, '-') << "\n\n";

	// Test 2: Environment with sample tiles
	std::cout << "Test 2: Environnement avec tuiles d'exemple\n";
	EnvJoueur joueur2("Bob", 3);

	// Create sample tiles (you can uncomment and modify these)

	// Example tile 1: Forest/Prairie mix with bear and elk
	std::array<Habitat, 6> habitats1 = {
		Habitat::foret, Habitat::montagne, Habitat::prairie,
		Habitat::prairie, Habitat::foret, Habitat::prairie
	};
	std::vector<Faune> faunes1 = { Faune::ours, Faune::wapiti };
	Tuile tuile1(habitats1, faunes1, false);
	Tuile& tuile1Ref = tuile1;

	// Example tile 2: River/Marsh with salmon
	std::array<Habitat, 6> habitats2 = {
		Habitat::fleuve, Habitat::fleuve, Habitat::marais,
		Habitat::marais, Habitat::fleuve, Habitat::marais
	};
	std::vector<Faune> faunes2 = { Faune::saumon };
	Tuile tuile2(habitats2, faunes2, true); // Gives nature token
	Tuile& tuile2Ref = tuile2;

	std::vector<Tuile> mesTuiles = { tuile1, tuile2, tuile1 };

	// Place tiles on the map
	joueur2.setTuilesDepart(mesTuiles);// Set initial tiles for the player
	//joueur2.placerTuileDefinitive(Position(0, 0, 0), tuile1);
	//joueur2.placerTuileDefinitive(Position(1, -1, 0), tuile2);

	// Place some wildlife tokens directly on tiles
	JetonFaune jetonOurs(Faune::ours);
	JetonFaune jetonSaumon(Faune::saumon);

	joueur2.placerJetonFaune(Position(0, 0, 0), jetonOurs);
	//std::cout << "\n\n\n\n\n";
	//std::cout << tuile1;
	//std::cout << "\n\n\n\n\n";
	joueur2.placerJetonFaune(Position(1, -1, 0), jetonSaumon);


	std::cout << joueur2 << "\n";
	std::cout << std::string(50, '-') << "\n\n";
	/*
	// Test 3: Environment with pending placement
	std::cout << "Test 3: Test des coordonnees et placement en attente\n";

	EnvJoueur joueur3("Charlie", 2, 10);

	// Test pending placement
	*/
	std::array<Habitat, 6> habitats3 = {
		Habitat::montagne, Habitat::montagne, Habitat::montagne,
		Habitat::montagne, Habitat::montagne, Habitat::montagne //TEST 3 IS THE PROBLEM
	};
	std::vector<Faune> faunes3 = { Faune::buse, Faune::renard };
	Tuile tuile3(habitats3, faunes3);
	
	
	// Place without confirming to test pending placement display
	//std::cout<<"\n\n\n position est valide?"<<joueur2.positionTuileValide(Position(1, 0, -1));
	//std::cout << "\n\n\n A tuile a 000?"<< joueur2.aTuile(Position(0,0,0));
	joueur2.placerTuile(Position(1, 0, -1), tuile3);
	std::cout << joueur2 << "\n";

	//std::cout << joueur3 << "\n";

}

void testDivers(){
	// Test 2: Environment with sample tiles
	std::cout << "Test 2: Environnement avec tuiles d'exemple\n";
	EnvJoueur joueur2("Bob", 3);

	// Example tile 1: Forest/Prairie mix with bear and elk
	std::array<Habitat, 6> habitats1 = {
		Habitat::foret, Habitat::montagne, Habitat::prairie,
		Habitat::prairie, Habitat::foret, Habitat::prairie
	};
	std::vector<Faune> faunes1 = { Faune::ours, Faune::wapiti };
	Tuile tuile1(habitats1, faunes1, false);
	Tuile& tuile1Ref = tuile1;

	// Example tile 2: River/Marsh with salmon
	std::array<Habitat, 6> habitats2 = {
		Habitat::fleuve, Habitat::fleuve, Habitat::marais,
		Habitat::marais, Habitat::fleuve, Habitat::marais
	};
	std::vector<Faune> faunes2 = { Faune::saumon };
	Tuile tuile2(habitats2, faunes2, true); // Gives nature token
	Tuile& tuile2Ref = tuile2;

	std::vector<Tuile> mesTuiles = { tuile1, tuile2, tuile1 };

	// Place tiles on the map
	joueur2.setTuilesDepart(mesTuiles);// Set initial tiles for the player
	//joueur2.placerTuileDefinitive(Position(0, 0, 0), tuile1);
	//joueur2.placerTuileDefinitive(Position(1, -1, 0), tuile2);

	JetonFaune jetonOurs(Faune::ours);
	JetonFaune jetonSaumon(Faune::saumon);

	joueur2.placerJetonFaune(Position(0, 0, 0), jetonOurs);
	std::cout << joueur2.placerJetonFaune(Position(1, -1, 0), jetonSaumon) << "\n\n";//False

	std::cout << joueur2;
	//std::cout << joueur2 << "\n";
	std::cout << std::string(50, '-') << "\n\n";
}

//void testEnvJoueur() {
//	using namespace GestionPieces;
//	std::vector <std::vector<Tuile>> td = instancierTuilesDepart();
//	std::vector<Tuile> t = instancierTuiles();
//
//	//std::cout << std::vector{t[0], t[1]};
//	//std::cout << td.at(0);
//	std::cout << t[0];
//}
