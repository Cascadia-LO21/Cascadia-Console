#include <iostream>
#include "carte_marquage.h"
#include <unordered_map>
#include <unordered_set>

int CarteSaumon::methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const {};

int CarteOurs::methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const {

	std::unordered_set<Position> PositionsVisitees;
	int nb_paires_ours = 0;

	//parcourt des tuiles de carte
	for (const auto& [position, tuile] : carte) { // [cle,valeur] appartenant à carte

		//on ignore la tuile si deja visitée
		if (PositionsVisitees.count(position) == 1) continue;
		//on ignre la tuile si pas de jeton faune ou jeton non ours, et on la marque comme visitee
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::ours) {
			PositionsVisitees.insert(position);
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
	int scoreTotal = 0;

	//parcourt des tuiles de carte
	for (const auto& [position, tuile] : carte) { // [cle,valeur] appartenant à carte

		//on ignore la tuile si deja visitée
		if (PositionsVisitees.count(position) == 1) continue;
		//on ignre la tuile si pas de jeton faune ou jeton non buse
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::buse) {
			PositionsVisitees.insert(position);
			continue;
		}

		//on vérifie s'il y a d'autres buses adjacents
		
		
	}
}