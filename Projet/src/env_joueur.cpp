#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <stdexcept>
#include <random>
#include "position.h"
#include "tuile.h"
#include "env_joueur.h"

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
	auto voisins = coord.getVecteurPositionsAdjacentes();
	for (const auto& v : voisins) {
		if (getTuile(v)) {
			return true;
		}
	}
	//aucune tuile adjacente (aucun voisin)
	return false;
};

void EnvJoueur::placerTuile(const Position& coord, const Tuile& tuile) {
	if (!positionTuileValide(coord)) {
		throw std::invalid_argument("Position invalide pour placer la tuile");
	}

	//si y'avait déjà une tentative de placement pour cette tuile, undo en premier
	if (placementEnAttente) {
		undoDernierPlacement();
	}

	//placer la tuile
	tuiles[coord] = tuile;

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

	//marquer dans la tuile que sa position est confirmée
	tuiles[dernierePosition.value()].confirmerPlacement();

	//vider les variables d'attente
	placementEnAttente = false;
	dernierePosition.reset(); //on n'a plus de position en attente
	derniereTuile.reset(); //on n'a plus de tuile en attente
};

bool EnvJoueur::undoDernierPlacement() {
	if (!placementEnAttente || !dernierePosition.has_value() || !derniereTuile.has_value()) {
		return false; //rien à annuler
	}

	//retirer la tuile de la map
	tuiles.erase(dernierePosition.value());

	//vider les variables d'attente
	placementEnAttente = false;
	dernierePosition.reset(); //on n'a plus de position en attente
	derniereTuile.reset(); //on n'a plus de tuile en attente

	return true; //annulation réussie
}

void EnvJoueur::placerTuileDefinitive(const Position& coord, const Tuile& tuile) {
	placerTuile(coord, tuile);
	confirmerPlacement();
}

//retourne -1 si le jeton ne peut être placé nulpart, 0 si le jeton ne peut etre placé à cette position, 1 si le jeton est placé avec succès
int EnvJoueur::placerJetonFaune(const Position& coord, const JetonFaune& jeton) {

	//étape 1: vérification primitive, est-ce qu'on peut placer le jeton sur une tuile quelconque dans la map?
	// (oui si il y a au moins une tuile qui a parmi son vecteur faunes le type du jeton passé en paramètre et
	// qui n'a pas de jeton déjà placé dessus)
	for (const auto& [pos, tuile] : tuiles) {
		if (!tuile.JetonFaunePresent()) { // est ce qu'il y a un jeton placé déjà sur cette tuile?
			const auto& faunes = tuile.getFaunes();
			if (!std::any_of(faunes.begin(), faunes.end(),
				[&](const JetonFaune& j) { return j.getType() == jeton.getType(); })) {// Vérifie si faunes contient le type du jeton entrée

				return -1; //jeton ne peut pas etre placé sur aucunes tuiles dans la map
				//IL FAUDRA PERMETTRE A L'UTILISATEUR DE L'ENLEVER DE LA PIOCHE, ou de choisir une autre paire tuile/jeton
			}
		}
	}

	//étape 2: est-ce qu'il y a une tuile confirmée libre à cette coordonnée?
	if (!aTuileConfirmee(coord)) {
		return 0; //jeton ne peut pas etre placé
	}
	Tuile* tuilePtr = getTuileNonConst(coord);
	if (!tuilePtr) {
		throw std::logic_error("Aucune tuile à cette position");
	}
	if (tuilePtr->JetonFaunePresent()) {
		return 0; //jeton ne peut pas etre placé, il y a déjà un jeton sur cette tuile
	}

	//étape 3: si la tuile à cette coordonnée a parmis son vecteur faunes le type du jeton passé en paramètre
	if (!std::any_of(tuilePtr->getFaunes().begin(), tuilePtr->getFaunes().end(), 
			[&](const JetonFaune& j) { return j.getType() == jeton.getType(); })) {
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
void EnvJoueur::setTuilesDepart(std::vector<std::vector<Tuile>>& tuilesDepart) {
	if (tuilesDepart.empty()) {
		throw std::invalid_argument("Le vecteur de tuiles de départ possibles est vide.");
	}

	// On vide les tuiles existantes
	tuiles.clear();

	// Générateur aléatoire moderne
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<size_t> distrib(0, tuilesDepart.size() - 1);
	int index = distrib(gen);
		// On ajoute les tuiles de départ
	const std::vector<Tuile>& selection = tuilesDepart[index];

	if (selection.size() < 3) {
		throw std::invalid_argument("La sélection de tuiles n'a pas 3 tuiles.");
	}

	// Placer les 3 tuiles selon les positions définies
	// Ordre attendu : haut, gauche, droite(centre)
	placerTuileDefinitive(Position(0,-1,1), selection[0]);
	placerTuileDefinitive(Position(-1, 0, 1), selection[1]);
	placerTuileDefinitive(Position(0, 0, 0), selection[2]);
}
