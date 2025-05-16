#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <stdexcept>
#include "position.h"
#include "tuile.h"
#include "env_joueur.h"


//get pointeur de la tuile à cette coordonnée
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