#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <stdexcept>
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

//est ce qu'on peut placer une tuile à cette coordonnée?
bool EnvJoueur::positionValide(const Position& coord) const {
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
	if (!positionValide(coord)) {
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