#include <iostream>
#include "carte_marquage.h"
#include <unordered_map>
#include <unordered_set>
#include <functional>

int CarteOurs::methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const {

	std::unordered_set<Position> PositionsVisitees;
	int nb_paires_ours = 0;

	//parcourt des tuiles de carte
	for (const auto& [position, tuile] : carte) { // [cle,valeur] appartenant à carte

		//on ignore la tuile si deja visitée
		if (PositionsVisitees.count(position) == 1) continue;
		//on ignre la tuile si pas de jeton faune ou jeton non ours, et on la marque comme visitee
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::ours) {
			continue;
		}

		//on vérifie s'il y a d'autres ours adjacents
		int nb_ours_adj =0;
		Position pos_ours_adj_unique; //sera instancié plus tard s'il y a un ours adjacent

		std::vector<Position> vec = position.getVecteurPositionsAdjacentes();

		for (unsigned int i = 0; i < 6; i++) {
			Position pos_voisine = vec[i];

			if (carte.count(pos_voisine) == 1 && PositionsVisitees.count(pos_voisine) == 0) {
				const Tuile& tuile_voisine = carte.at(pos_voisine);

				if (tuile_voisine.JetonFaunePresent() && tuile_voisine.getFaunePlace() == Faune::ours) {
					nb_ours_adj++;
					pos_ours_adj_unique = pos_voisine;
				}
				PositionsVisitees.insert(pos_voisine);
			}
		}
		PositionsVisitees.insert(position); //position actuelle

		//on vérifie si l'unique ours adj à la position courante n'est pas adjacent à un autre ours
		if (nb_ours_adj == 1) {
			int nb_ours_adj_2 = 0; 
			std::vector<Position> vec = pos_ours_adj_unique.getVecteurPositionsAdjacentes();
			for (unsigned int i = 0; i < 6; i++) {
				Position pos_voisine = vec[i];
				if (carte.count(pos_voisine) == 1) { //il ne faut pas vérifier si elle a déjà été visitée
					const Tuile& tuile_voisine = carte.at(pos_voisine);
					if (tuile_voisine.JetonFaunePresent() && tuile_voisine.getFaunePlace() == Faune::ours) {
						nb_ours_adj_2++;
						PositionsVisitees.insert(pos_voisine);
					}
				}
			}
			if (nb_ours_adj_2 == 1) {
				nb_paires_ours++;
			}
		}

	}

	//Calcul du score total
	switch (nb_paires_ours) {
	case 0: return 0;
	case 1: return 4;
	case 2: return 11;
	case 3: return 19;
	default: return 27; // 4+ paires d'ours
	}
}

int CarteBuse::methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const {
	//initialisations
	std::unordered_set<Position> PositionsVisitees;
	int nb_buses_isolees = 0;

	//parcourt des tuiles de carte
	for (const auto& [position, tuile] : carte) { // [cle,valeur] appartenant à carte

		//on ignore la tuile si deja visitée
		if (PositionsVisitees.count(position) == 1) continue;
		//on ignre la tuile si pas de jeton faune ou jeton non buse
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::buse) {
			continue;
		}

		//on vérifie s'il y a d'autres buses adjacentes
		int nb_buses_adj = 0;
		std::vector<Position> vec = position.getVecteurPositionsAdjacentes();
		for (unsigned int i = 0; i < 6; i++) {
			Position pos_voisine = vec[i];

			if (carte.count(pos_voisine) == 1) {
				const Tuile& tuile_voisine = carte.at(pos_voisine);
				PositionsVisitees.insert(pos_voisine);

				if (tuile_voisine.JetonFaunePresent() && tuile_voisine.getFaunePlace() == Faune::buse) {
					nb_buses_adj++;
				}
			}
		}
		PositionsVisitees.insert(position); //position actuelle

		if (nb_buses_adj == 0) {
			nb_buses_isolees++;
		}
	}

	//Calcul du score total
	switch (nb_buses_isolees) {
	case 0: return 0;
	case 2: return 5;
	case 3: return 8;
	case 4: return 11;
	case 5: return 14;
	case 6: return 18;
	case 7: return 22;
	default: return 26; //8+ buses
	}
}



int CarteRenard::methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const
{
	int scoreTotal = 0;
	std::unordered_set<Position> PositionsVisitees;
	std::vector<int> AdjRenards;

	//parcourt des tuiles de carte
	for (const auto& [position, tuile] : carte) { 
		std::unordered_set<Faune> FaunesAdjacentes; //afin de stocker les faunes sans duplication, puis avoir la taille

		//on ignore la tuile si deja visitée
		if (PositionsVisitees.count(position) == 1) continue;
		//on ignre la tuile si pas de jeton faune ou jeton non renard
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::renard) {
			continue;
		}

		//parcourir les tuiles adjacentes à la tuile renard
		std::vector<Position> vec = position.getVecteurPositionsAdjacentes();
		for (unsigned int i = 0; i < 6; i++) {

			Position pos_voisine = vec[i];
			if (carte.count(pos_voisine) == 1) {
				const Tuile& tuile_voisine = carte.at(pos_voisine);

				if (tuile_voisine.JetonFaunePresent()) {
					FaunesAdjacentes.insert(tuile_voisine.getFaunePlace());
				}
			}
		}
		//on compte le nombre de types adj pour chaque renard et on l'ajoute à AdjRenards
		AdjRenards.push_back(FaunesAdjacentes.size());
		PositionsVisitees.insert(position);
	}
	//insert boucle pour parcourir le vecteur ou set, puis le switch
	for (int renard : AdjRenards) {
		switch (renard) {
		case 1: scoreTotal += 1; break;
		case 2: scoreTotal += 2; break;
		case 3: scoreTotal += 3; break;
		case 4: scoreTotal += 4; break;
		case 5: scoreTotal += 5; break;
		default: break; //cas 0
		}
	}
	return scoreTotal;
}

int CarteWapiti::methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const
{
	//penser aux methodes get Q, get R, get S pour les lignes
	//SURTOUT la fct cote tangeant
	return 0;
}

int CarteSaumon::methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const
{
	std::unordered_set<Position> PositionsVisitees;
	std::vector<int> taillesChaines;
	int scoreTotal = 0;

	for (const auto& [position, tuile] : carte) {
		int longueur_chaine = 0;

		//on ignore la tuile si deja visitée
		if (PositionsVisitees.count(position) == 1) continue;
		//on ignore la tuile si pas de jeton faune ou jeton non saumon
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::saumon) {
			continue;
		}

		//position actuelle contient saumon
		int tailleChaine = explorerChaineA(carte, position,PositionsVisitees, nullptr); //nullptr car premier element n'a pas de pere
		if (tailleChaine > 0) {
			taillesChaines.push_back(tailleChaine);
		}
	}

	for (int chaine : taillesChaines) {
		switch (chaine) {
		case 0: scoreTotal += 0; break;
		case 1: scoreTotal += 2; break;
		case 2: scoreTotal += 4; break;
		case 3: scoreTotal += 7; break;
		case 4: scoreTotal += 11; break;
		case 5: scoreTotal += 15; break;
		case 6: scoreTotal += 20; break;
		default: scoreTotal += 26; break; // pour 7+ 
		}
	}
	return scoreTotal;
}

int CarteSaumon::explorerChaineA(const std::unordered_map<Position,Tuile>& carte,const Position& position, std::unordered_set<Position>& PositionsVisitees, const Position* pos_pere) const
{
	PositionsVisitees.insert(position);
	int nb_saumons_adj = 0;
	int taille = 1; //la chaine contient au moins 1 saumon (current)

	std::vector<Position> vec = position.getVecteurPositionsAdjacentes();

	//parcourir les positions adjacentes
	for (const Position& pos_voisine : vec) {
		//verif pr ne pas faire le chemin inverse
		if (pos_pere && pos_voisine == *pos_pere) {
			continue;
		}
		//verif si elle existe bien dans la carte
		if (carte.count(pos_voisine) == 0) continue;
		const Tuile& tuile_voisine = carte.at(pos_voisine);

		if (tuile_voisine.JetonFaunePresent() && tuile_voisine.getFaunePlace() == Faune::saumon) {
			nb_saumons_adj++;
			if (nb_saumons_adj > 2) return 0; //chaine annulée
			if (PositionsVisitees.count(pos_voisine) == 0) {
				//appel récursif sur la position voisine, et la position actuelle est l'argument pere
				int taille_suite = explorerChaineA(carte, pos_voisine, PositionsVisitees, &position);
				if (taille_suite == 0) return 0; //la chaine est invalide, donc on arrete
				//else
				taille += taille_suite;
			}
		}
	}
	
	return taille;
}
