#include <iostream>
#include "carte_marquage.h"
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <queue>

int CarteOurs::methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const {

	std::unordered_set<Position> PositionsVisitees;
	int nbPairesOurs = 0;

	//parcourt des tuiles de carte
	for (const auto& [position, tuile] : carte) { // [cle,valeur] appartenant à carte

		//on ignore la tuile si deja visitée
		if (PositionsVisitees.count(position) == 1) continue;
		//on ignre la tuile si pas de jeton faune ou jeton non ours, et on la marque comme visitee
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::ours) continue;

		//on vérifie s'il y a d'autres ours adjacents
		int nbOursAdj =0;
		Position posOursAdjUnique; //sera instancié plus tard s'il y a un ours adjacent

		std::vector<Position> vec = position.getVecteurPositionsAdjacentes();

		for (unsigned int i = 0; i < 6; i++) {
			Position posVoisine = vec[i];

			if (carte.count(posVoisine) == 1 && PositionsVisitees.count(posVoisine) == 0) {
				const Tuile& tuileVoisine = carte.at(posVoisine);

				if (tuileVoisine.JetonFaunePresent() && tuileVoisine.getFaunePlace() == Faune::ours) {
					nbOursAdj++;
					posOursAdjUnique = posVoisine;
				}
				PositionsVisitees.insert(posVoisine);
			}
		}
		PositionsVisitees.insert(position); //position actuelle

		//on vérifie si l'unique ours adj à la position courante n'est pas adjacent à un autre ours
		if (nbOursAdj == 1) {
			int nbOursAdj2 = 0; 
			std::vector<Position> vec = posOursAdjUnique.getVecteurPositionsAdjacentes();
			for (unsigned int i = 0; i < 6; i++) {

				Position posVoisine = vec[i];
				if (carte.count(posVoisine) == 1) { //il ne faut pas vérifier si elle a déjà été visitée
					const Tuile& tuile_voisine = carte.at(posVoisine);
					if (tuile_voisine.JetonFaunePresent() && tuile_voisine.getFaunePlace() == Faune::ours) {
						nbOursAdj2++;
						PositionsVisitees.insert(posVoisine);
					}
				}
			}
			if (nbOursAdj2 == 1) {
				nbPairesOurs++;
			}
		}
	}
	//Calcul du score total
	switch (nbPairesOurs) {
	case 0: return 0;
	case 1: return 4;
	case 2: return 11;
	case 3: return 19;
	default: return 27; // 4+ paires d'ours
	}
}

int CarteOurs::methodeCalculB(const std::unordered_map<Faune, std::unordered_set<Position>>& carte) const
{
	if (carte.count(Faune::ours) == 0) return 0;

	const auto& positionsOurs = carte.at(Faune::ours); //recup les positions ours
	std::unordered_set<Position> positionsVisitees;

	for (const Position& pos : positionsOurs) {
		if (positionsVisitees.count(pos)) continue;

		std::unordered_set<Position> groupe;
		std::queue<Position> file;
		file.push(pos);

		while (!file.empty()) {
			Position current = file.front();
			file.pop();

			if (positionsVisitees.count(current)) continue;
			positionsVisitees.insert(current);
			groupe.insert(current);

			for (const Position& voisine : current.getVecteurPositionsAdjacentes()) {
				// s'assurer que voisine est ours
				if (positionsOurs.count(voisine) && !positionsVisitees.count(voisine)) { 
					file.push(voisine); //
				}
			}
		}

		if (groupe.size() == 3) {
			return 10;
		}
	}

	// aucun groupe de 3 ours trouvé
	return 0;
}


int CarteBuse::methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const {
	//initialisations
	std::unordered_set<Position> PositionsVisitees;
	int nbBusesIsolees = 0;

	//parcourt des tuiles de carte
	for (const auto& [position, tuile] : carte) { // [cle,valeur] appartenant à carte

		//on ignore la tuile si deja visitée
		if (PositionsVisitees.count(position) == 1) continue;
		//on ignre la tuile si pas de jeton faune ou jeton non buse
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::buse) continue;

		//on vérifie s'il y a d'autres buses adjacentes
		int nbBusesAdj = 0;
		std::vector<Position> vec = position.getVecteurPositionsAdjacentes();
		for (unsigned int i = 0; i < 6; i++) {
			Position posVoisine = vec[i];

			if (carte.count(posVoisine) == 1) {
				const Tuile& tuileVoisine = carte.at(posVoisine);
				PositionsVisitees.insert(posVoisine);

				if (tuileVoisine.JetonFaunePresent() && tuileVoisine.getFaunePlace() == Faune::buse) {
					nbBusesAdj++;
				}
			}
		}
		PositionsVisitees.insert(position); //position actuelle

		if (nbBusesAdj == 0) {
			nbBusesIsolees++;
		}
	}

	//Calcul du score total
	switch (nbBusesIsolees) {
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

int CarteBuse::methodeCalculB(const std::unordered_map<Faune, std::unordered_set<Position>>& carte) const
{

	
	return 0;
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
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::renard) continue;

		//parcourir les tuiles adjacentes à la tuile renard
		std::vector<Position> vec = position.getVecteurPositionsAdjacentes();
		for (unsigned int i = 0; i < 6; i++) {

			Position posVoisine = vec[i];
			if (carte.count(posVoisine) == 1) {
				const Tuile& tuileVoisine = carte.at(posVoisine);

				if (tuileVoisine.JetonFaunePresent()) {
					FaunesAdjacentes.insert(tuileVoisine.getFaunePlace());
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

int CarteRenard::methodeCalculB(const std::unordered_map<Faune, std::unordered_set<Position>>& carte) const
{
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
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::saumon) continue;


		//position actuelle contient saumon
		int tailleChaine = explorerChaineSaumonA(carte, position,PositionsVisitees, nullptr); //nullptr car premier element n'a pas de pere
		if (tailleChaine > 0) {
			taillesChaines.push_back(tailleChaine);
		}
	}

	for (int chaine : taillesChaines) {
		switch (chaine) {
		case 0: scoreTotal += 0; break;
		case 1: scoreTotal += 2; break;
		case 2: scoreTotal += 5; break;
		case 3: scoreTotal += 9; break;
		default: scoreTotal += 13; break; // pour 4+ 
		}
	}
	return scoreTotal;
}

int CarteSaumon::explorerChaineSaumonA(const std::unordered_map<Position,Tuile>& carte,const Position& position, std::unordered_set<Position>& PositionsVisitees, const Position* pos_pere) const
{
	PositionsVisitees.insert(position);
	int nbSaumonsAdj = 0;
	int taille = 1; //la chaine contient au moins 1 saumon (current)

	std::vector<Position> vec = position.getVecteurPositionsAdjacentes();

	//parcourir les positions adjacentes
	for (const Position& posVoisine : vec) {
		//verif pr ne pas faire le chemin inverse
		if (pos_pere && posVoisine == *pos_pere) {
			continue;
		}
		//verif si elle existe bien dans la carte
		if (carte.count(posVoisine) == 0) continue;
		const Tuile& tuileVoisine = carte.at(posVoisine);

		if (!tuileVoisine.JetonFaunePresent() && tuileVoisine.getFaunePlace() != Faune::saumon) continue;

		nbSaumonsAdj++;
		if (nbSaumonsAdj > 2) return 0; //chaine annulée
		if (PositionsVisitees.count(posVoisine) == 0) {
			//appel récursif sur la position voisine, et la position actuelle est l'argument pere
			int tailleSuite = explorerChaineSaumonA(carte, posVoisine, PositionsVisitees, &position);
			if (tailleSuite == 0) return 0; //la chaine est invalide, donc on arrete
			//else
			taille += tailleSuite;
		}
	}
	
	return taille;
}

int CarteSaumon::methodeCalculB(const std::unordered_map<Faune, std::unordered_set<Position>>& carte) const
{
	if (carte.count(Faune::saumon) == 0) return 0; // pas de saumon du tout

	const auto& positionsSaumon = carte.at(Faune::saumon); // recup liste des saumons
	std::unordered_set<Position> positionsVisitees;
	std::vector<int> taillesChaines;
	int scoreTotal = 0;

	for (const Position& position : positionsSaumon) {
		if (positionsVisitees.count(position)) continue;

		int tailleChaine = explorerChaineSaumonB(carte, position, positionsVisitees, nullptr);
		if (tailleChaine > 0) {
			taillesChaines.push_back(tailleChaine);
		}
	}

	for (int chaine : taillesChaines) {
		switch (chaine) {
		case 0: scoreTotal += 0; break;
		case 1: scoreTotal += 2; break;
		case 2: scoreTotal += 4; break;
		case 3: scoreTotal += 9; break;
		case 4: scoreTotal += 11; break;
		default: scoreTotal += 17; break; // 5+
		}
	}
	return scoreTotal;
}
int CarteSaumon::explorerChaineSaumonB(const std::unordered_map<Faune, std::unordered_set<Position>>& carte,
	const Position& position,
	std::unordered_set<Position>& positionsVisitees,
	const Position* pos_pere) const
{
	if (carte.count(Faune::saumon) == 0) return 0;

	const auto& positionsSaumon = carte.at(Faune::saumon);

	positionsVisitees.insert(position);
	int nbSaumonsAdj = 0;
	int taille = 1;

	for (const Position& posVoisine : position.getVecteurPositionsAdjacentes()) {
		if (pos_pere && posVoisine == *pos_pere) continue;
		if (positionsSaumon.count(posVoisine) == 0) continue; // Pas de saumon à cette position

		nbSaumonsAdj++;
		if (nbSaumonsAdj > 2) return 0; // Chaine invalide

		if (positionsVisitees.count(posVoisine) == 0) {
			int tailleSuite = explorerChaineSaumonB(carte, posVoisine, positionsVisitees, &position);
			if (tailleSuite == 0) return 0;
			taille += tailleSuite;
		}
	}

	return taille;
}

int CarteWapiti::methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const
{
	std::unordered_set<Position> PositionsVisitees;
	std::vector<int> taillesChaines;
	int scoreTotal = 0;

	for (const auto& [position, tuile] : carte) {
		int longueur_chaine = 0;

		//on ignore la tuile si deja visitée
		if (PositionsVisitees.count(position) == 1) continue;
		//on ignore la tuile si pas de jeton faune ou jeton non wapiti
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::wapiti) continue;

		//position actuelle contient saumon
		int tailleChaine = explorerChaineWapitiA(carte, position, PositionsVisitees, std::nullopt); //nullopt car on n'a pas encore trouvé la direction de la chaine
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

int CarteWapiti::explorerChaineWapitiA(const std::unordered_map<Position, Tuile>& carte, const Position& position, std::unordered_set<Position>& PositionsVisitees, std::optional<Direction> direction) const
{
	if (PositionsVisitees.count(position)) return 0;
	if (carte.count(position) == 0) return 0;

	const Tuile& tuile = carte.at(position);
	if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::wapiti) return 0;

	PositionsVisitees.insert(position);

	// cas 0: on cherche une direction et on explore dans les 2 sens
	if (!direction.has_value()) {
		for (const Position& posVoisine : position.getVecteurPositionsAdjacentes()) {
			if (carte.count(posVoisine) == 0) continue;

			const Tuile& tVoisine = carte.at(posVoisine);
			if (!tVoisine.JetonFaunePresent() || tVoisine.getFaunePlace() != Faune::wapiti) continue;

			//trouvé un wapiti voisin
			Direction dir = coteTangent(position, posVoisine);
			Direction opp = getDirectionOpposee(dir);

			int taille1 = explorerChaineWapitiA(carte, posVoisine, PositionsVisitees, dir);
			int taille2 = explorerChaineWapitiA(carte, position.getPositionAdjacente(opp), PositionsVisitees, opp);

			return 1 + taille1 + taille2; //1 : la pos actuelle compte + chaine dans une direction + chaine direction opposee
		}
		//pas de voisin wapiti, alors juste la pos actuelle
		return 1;
	}

	//else, on a déjà la direction, on continue à la parcourir
	Position suivante = position.getPositionAdjacente(direction.value());
	return 1 + explorerChaineWapitiA(carte, suivante, PositionsVisitees, direction);
	
}

int CarteWapiti::methodeCalculB(const std::unordered_map<Faune, std::unordered_set<Position>>& carte) const
{
	return 0;
}


int main() {
	std::unordered_map<Position, Tuile> carteTest;

	//generer des tuiles
	/*
	std::cout << "Score Ours (A) : " << carteOurs.methodeCalculA(carteTest) << std::endl;
	std::cout << "Score Buse (A) : " << carteBuse.methodeCalculA(carteTest) << std::endl;
	std::cout << "Score Renard (A) : " << carteRenard.methodeCalculA(carteTest) << std::endl;
	std::cout << "Score Saumon (A) : " << carteSaumon.methodeCalculA(carteTest) << std::endl;
	std::cout << "Score Wapiti (A) : " << carteWapiti.methodeCalculA(carteTest) << std::endl;
	*/
	return 0;
}