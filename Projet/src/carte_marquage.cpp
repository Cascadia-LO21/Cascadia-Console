#include <iostream>
#include "carte_marquage.h"
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <queue>

// Saumon A
int CarteSaumonA::CalculScore(const EnvJoueur& envJ) const
{
	const auto& carte = envJ.getTuiles();  // accès à la carte via EnvJoueur
	std::unordered_set<Position> positionsVisitees;
	std::vector<int> taillesChaines;
	int scoreTotal = 0;

	for (const auto& [position, tuile] : carte) {
		if (positionsVisitees.count(position) == 1) continue;
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::saumon) continue;

		int tailleChaine = explorerChaine(carte, position, positionsVisitees, nullptr);
		if (tailleChaine > 0) {
			taillesChaines.push_back(tailleChaine);
		}
	}

	for (int chaine : taillesChaines) {
		switch (chaine) {
		case 0: scoreTotal += 0; break;
		case 1: scoreTotal += 2; break;
		case 2: scoreTotal += 5; break;
		case 3: scoreTotal += 8; break;
		case 4: scoreTotal += 12; break;
		case 5: scoreTotal += 16; break;
		case 6: scoreTotal += 20; break;
		default: scoreTotal += 25; break; //7 +
		}
	}
	return scoreTotal;
}

int CarteSaumonA::explorerChaine(const std::unordered_map<Position, Tuile>& carte, const Position& position, std::unordered_set<Position>& PositionsVisitees, const Position* parent) const
{
	PositionsVisitees.insert(position);
	int nbSaumonsAdj = 0;
	int taille = 1; //la chaine contient au moins 1 saumon (current)

	std::vector<Position> vec = position.getVecteurPositionsAdjacentes();

	//parcourir les positions adjacentes
	for (const Position& posVoisine : vec) {
		//verif pr ne pas faire le chemin inverse
		if (parent && posVoisine == *parent) {
			continue;
		}
		//verif si elle existe bien dans la carte
		if (carte.count(posVoisine) == 0) continue;
		const Tuile& tuileVoisine = carte.at(posVoisine);

		if (!tuileVoisine.JetonFaunePresent() || tuileVoisine.getFaunePlace() != Faune::saumon) continue;

		nbSaumonsAdj++;
		if (nbSaumonsAdj > 2) return 0; //chaine annulée
		if (PositionsVisitees.count(posVoisine) == 0) {
			//appel récursif sur la position voisine, et la position actuelle est l'argument pere
			int tailleSuite = explorerChaine(carte, posVoisine, PositionsVisitees, &position);
			if (tailleSuite == 0) return 0; //la chaine est invalide, donc on arrete
			//else
			taille += tailleSuite;
		}
	}

	return taille;
}

// Saumon B
int CarteSaumonB::CalculScore(const EnvJoueur& envJ) const
{
	const auto& mapPositionsJetons = envJ.getMapPositionsJetons();

	if (mapPositionsJetons.count(Faune::saumon) == 0) return 0; // pas de saumon du tout

	const auto& positionsSaumon = mapPositionsJetons.at(Faune::saumon);
	std::unordered_set<Position> PositionsVisitees;
	std::vector<int> taillesChaines;
	int scoreTotal = 0;

	for (const Position& position : positionsSaumon) {
		if (PositionsVisitees.count(position)) continue;

		int tailleChaine = explorerChaine(mapPositionsJetons, position, PositionsVisitees, nullptr);
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

int CarteSaumonB::explorerChaine(const std::unordered_map<Faune, std::unordered_set<Position>>& carte, const Position& position, std::unordered_set<Position>& PositionsVisitees, const Position* parent) const
{
	if (carte.count(Faune::saumon) == 0) return 0;

	const auto& positionsSaumon = carte.at(Faune::saumon);

	PositionsVisitees.insert(position);
	int nbSaumonsAdj = 0;
	int taille = 1;

	for (const Position& posVoisine : position.getVecteurPositionsAdjacentes()) {
		if (parent && posVoisine == *parent) continue;
		if (positionsSaumon.count(posVoisine) == 0) continue; // Pas de saumon à cette position

		nbSaumonsAdj++;
		if (nbSaumonsAdj > 2) return 0; // Chaine invalide

		if (PositionsVisitees.count(posVoisine) == 0) {
			int tailleSuite = explorerChaine(carte, posVoisine, PositionsVisitees, &position);
			if (tailleSuite == 0) return 0;
			taille += tailleSuite;
		}
	}

	return taille;
}

// Saumon C
int CarteSaumonC::CalculScore(const EnvJoueur& envJ) const
{
	const auto& mapPositionsJetons = envJ.getMapPositionsJetons();

	if (mapPositionsJetons.count(Faune::saumon) == 0) return 0; // pas de saumon du tout

	const auto& positionsSaumon = mapPositionsJetons.at(Faune::saumon);
	std::unordered_set<Position> PositionsVisitees;
	std::vector<int> taillesChaines;
	int scoreTotal = 0;

	for (const Position& position : positionsSaumon) {
		if (PositionsVisitees.count(position)) continue;

		int tailleChaine = explorerChaine(mapPositionsJetons, position, PositionsVisitees, nullptr);
		if (tailleChaine > 0) {
			taillesChaines.push_back(tailleChaine);
		}
	}

	for (int chaine : taillesChaines) {
		switch (chaine) {
		case 0: scoreTotal += 0; break;
		case 1: scoreTotal += 0; break;
		case 2: scoreTotal += 0; break;
		case 3: scoreTotal += 10; break;
		case 4: scoreTotal += 12; break;
		default: scoreTotal += 15; break; // 5+
		}
	}
	return scoreTotal;
}

int CarteSaumonC::explorerChaine(const std::unordered_map<Faune, std::unordered_set<Position>>& carte, const Position& position, std::unordered_set<Position>& PositionsVisitees, const Position* parent) const
{
	if (carte.count(Faune::saumon) == 0) return 0;

	const auto& positionsSaumon = carte.at(Faune::saumon);

	PositionsVisitees.insert(position);
	int nbSaumonsAdj = 0;
	int taille = 1;

	for (const Position& posVoisine : position.getVecteurPositionsAdjacentes()) {
		if (parent && posVoisine == *parent) continue;
		if (positionsSaumon.count(posVoisine) == 0) continue; // Pas de saumon à cette position

		nbSaumonsAdj++;
		if (nbSaumonsAdj > 2) return 0; // Chaine invalide

		if (PositionsVisitees.count(posVoisine) == 0) {
			int tailleSuite = explorerChaine(carte, posVoisine, PositionsVisitees, &position);
			if (tailleSuite == 0) return 0;
			taille += tailleSuite;
		}
	}

	return taille;
}

// Ours A

int CarteOursA::CalculScore(const EnvJoueur& envJ) const
{
	const auto& carte = envJ.getTuiles();
	std::unordered_set<Position> positionsVisitees;
	int nbPairesOurs = 0;

	for (const auto& [position, tuile] : carte) {
		if (positionsVisitees.count(position) > 0) continue;
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::ours) continue;

		positionsVisitees.insert(position);  // Marquer la position courante visitée

		int nbOursAdj = 0;
		Position posOursAdjUnique;

		// Chercher les ours adjacents non visités
		for (const Position& posVoisine : position.getVecteurPositionsAdjacentes()) {
			if (positionsVisitees.count(posVoisine) > 0) continue;
			auto it = carte.find(posVoisine);
			if (it == carte.end()) continue;

			const Tuile& tuileVoisine = it->second;
			if (tuileVoisine.JetonFaunePresent() && tuileVoisine.getFaunePlace() == Faune::ours) {
				nbOursAdj++;
				posOursAdjUnique = posVoisine;
			}
		}

		// Si un seul ours adjacent, vérifier ses adjacents
		if (nbOursAdj == 1) {
			int nbOursAdj2 = 0;
			for (const Position& posVoisine2 : posOursAdjUnique.getVecteurPositionsAdjacentes()) {
				auto it2 = carte.find(posVoisine2);
				if (it2 == carte.end()) continue;

				const Tuile& tuileVoisine2 = it2->second;
				if (tuileVoisine2.JetonFaunePresent() && tuileVoisine2.getFaunePlace() == Faune::ours) {
					nbOursAdj2++;
					positionsVisitees.insert(posVoisine2);
				}
			}
			// Ne compter que les paires (exactement 1 ours adjacent au second ours)
			if (nbOursAdj2 == 1) {
				nbPairesOurs++;
			}
		}
	}

	switch (nbPairesOurs) {
	case 0: return 0;
	case 1: return 4;
	case 2: return 11;
	case 3: return 19;
	default: return 27;
	}
}

// Ours B

int CarteOursB::CalculScore(const EnvJoueur& envJ) const
{
	const auto& mapPositionsJetons = envJ.getMapPositionsJetons();

	if (mapPositionsJetons.count(Faune::ours) == 0) return 0;

	const auto& positionsOurs = mapPositionsJetons.at(Faune::ours);
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
				// s'assurer que la voisine est dans positionsOurs et non visitée
				if (positionsOurs.count(voisine) && !positionsVisitees.count(voisine)) {
					file.push(voisine);
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

// Ours C

int CarteOursC::CalculScore(const EnvJoueur& envJ) const
{
	const auto& mapPositionsJetons = envJ.getMapPositionsJetons();
	if (mapPositionsJetons.count(Faune::ours) == 0) return 0;

	const auto& positionsOurs = mapPositionsJetons.at(Faune::ours);
	std::unordered_set<Position> dejaVisite;
	int nbF1 = 0, nbF2 = 0, nbF3 = 0;

	for (const Position& pos : positionsOurs) {
		if (dejaVisite.count(pos)) continue;

		// Parcours BFS pour trouver la composante connexe
		std::queue<Position> file;
		std::vector<Position> groupe;
		file.push(pos);
		dejaVisite.insert(pos);

		while (!file.empty()) {
			Position current = file.front();
			file.pop();
			groupe.push_back(current);

			for (const Position& voisine : current.getVecteurPositionsAdjacentes()) {
				if (positionsOurs.count(voisine) && !dejaVisite.count(voisine)) {
					file.push(voisine);
					dejaVisite.insert(voisine);
				}
			}
		}

		// Taille du groupe trouvé
		size_t taille = groupe.size();
		if (taille == 1) nbF1++;
		else if (taille == 2) nbF2++;
		else if (taille == 3) nbF3++;
		// Si plus de 3 on ignore
	}

	int scoreTotal = nbF1 * 2 + nbF2 * 5 + nbF3 * 8;
	if (nbF1 > 0 && nbF2 > 0 && nbF3 > 0) scoreTotal += 3;
	return scoreTotal;
}

// Buse A

int CarteBuseA::CalculScore(const EnvJoueur& envJ) const
{
	const auto& mapPositionsJetons = envJ.getMapPositionsJetons();
	if (mapPositionsJetons.count(Faune::buse) == 0) return 0; // pas de buse

	const auto& positionsBuse = mapPositionsJetons.at(Faune::buse);
	const auto& carte = envJ.getTuiles();
	int nbBusesValides = 0;

	for (const Position& position : positionsBuse) {
		bool buseAdj = false;

		for (const Position& posVoisine : position.getVecteurPositionsAdjacentes()) {
			if (positionsBuse.count(posVoisine)) {
				buseAdj = true;
				break;  // On sort de la boucle des positions voisines
			}
		}

		if (buseAdj) continue;
		nbBusesValides++;
	}

	switch (nbBusesValides) {
	case 0: return 0;
	case 1: return 2;
	case 2: return 5;
	case 3: return 8;
	case 4: return 11;
	case 5: return 14;
	case 6: return 18;
	case 7: return 22;
	default: return 26; // 8+ buses isolées
	}
}

// Buse B

int CarteBuseB::CalculScore(const EnvJoueur& envJ) const
{
	const auto& mapPositionsJetons = envJ.getMapPositionsJetons();
	if (mapPositionsJetons.count(Faune::buse) == 0) return 0; // pas de buse

	const auto& positionsBuse = mapPositionsJetons.at(Faune::buse);
	const auto& carte = envJ.getTuiles();
	int nbBusesValides = 0;
	std::vector<Position> busesAdjBuse;

	for (const Position& position : positionsBuse) {
		bool buseAdj = false;

		for (const Position& posVoisine : position.getVecteurPositionsAdjacentes()) {
			if (positionsBuse.count(posVoisine)) {
				buseAdj = true;
				busesAdjBuse.push_back(posVoisine);
				break;  // On sort de la boucle des positions voisines
			}

		}
		//si on a trouvé une buse adjacente, on passe à la prochaine buse à analyser
		if (buseAdj) continue;
		
		//Maintenant on vérifier la ligne de vision avec une autre buse
		bool ligneVue = false;
		int q = position.getQ();
		int r = position.getR();
		int s = position.getS();

		for (const Position& posLigne : positionsBuse) {
			if (posLigne == position || std::find(busesAdjBuse.begin(), busesAdjBuse.end(), posLigne) != busesAdjBuse.end()) continue;  // on ignore la position elle-meme
			if (q == posLigne.getQ() || r == posLigne.getR() || s == posLigne.getS()) {
				ligneVue = true;
				break;
			}	
		}
		if (ligneVue) nbBusesValides++;
	}

	switch (nbBusesValides) {
	case 0: return 0;
	case 1: return 0;
	case 2: return 5;
	case 3: return 9;
	case 4: return 12;
	case 5: return 16;
	case 6: return 20;
	case 7: return 24;
	default: return 28; // 8+ buses valides
	}
}

// Buse C

int CarteBuseC::CalculScore(const EnvJoueur& envJ) const
{
	const auto& mapPositionsJetons = envJ.getMapPositionsJetons();
	if (mapPositionsJetons.count(Faune::buse) == 0) return 0;

	const auto& positionsBuse = mapPositionsJetons.at(Faune::buse);
	std::vector<Position> busesIsolees;

	// recuperer les buses isolées
	for (const Position& buse : positionsBuse) {
		bool buseAdj = false;
		for (const Position& voisine : buse.getVecteurPositionsAdjacentes()) {
			if (positionsBuse.count(voisine)) {
				buseAdj = true;
				break;
			}
		}
		if (!buseAdj) {
			busesIsolees.push_back(buse);
		}
	}

	// compter les lignes de vue entre buses isolees
	int nbLignesVue = 0;
	for (size_t i = 0; i < busesIsolees.size(); ++i) {
		const Position& p1 = busesIsolees[i];
		for (size_t j = i + 1; j < busesIsolees.size(); ++j) {
			const Position& p2 = busesIsolees[j];
			if (p1.getQ() == p2.getQ() || p1.getR() == p2.getR() || p1.getS() == p2.getS()) {
				nbLignesVue++;
			}
		}
	}
	return 3 * nbLignesVue;

}

// Renard A

int CarteRenardA::CalculScore(const EnvJoueur& envJ) const
{
	int scoreTotal = 0;
	std::unordered_set<Position> PositionsVisitees;
	std::vector<int> AdjRenards;

	const auto& carte = envJ.getTuiles(); 

	for (const auto& [position, tuile] : carte) {
		if (PositionsVisitees.count(position) == 1) continue;
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::renard) continue;

		std::unordered_set<Faune> FaunesAdjacentes;

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

		AdjRenards.push_back(static_cast<int>(FaunesAdjacentes.size()));
		PositionsVisitees.insert(position);
	}

	for (int renard : AdjRenards) {
		switch (renard) {
		case 1: scoreTotal += 1; break;
		case 2: scoreTotal += 2; break;
		case 3: scoreTotal += 3; break;
		case 4: scoreTotal += 4; break;
		case 5: scoreTotal += 5; break;
		default: break;
		}
	}

	return scoreTotal;
}

// Renard B

int CarteRenardB::CalculScore(const EnvJoueur& envJ) const {
	const auto& mapPositionsJetons = envJ.getMapPositionsJetons();
	if (mapPositionsJetons.count(Faune::renard) == 0) return 0; // pas de renard

	const auto& positionsRenard = mapPositionsJetons.at(Faune::renard);
	const auto& carte = envJ.getTuiles();

	int scoreTotal = 0;
	std::vector<int> PairesAdj; //liste du nombre de paires adj par renard (int entre 0 et 3)

	for (const Position& position : positionsRenard) {
		int nbPaires = 0;
		std::unordered_map<Faune, int> FaunesAdj;
		// itération sur les positions voisines à 'position'
		for (const Position& posVoisine : position.getVecteurPositionsAdjacentes()) {
			
			// vérifier que la tuile existe
			if (!envJ.aTuileConfirmee(posVoisine)) continue;
			// récupérer la tuile, pas besoin de voir si != nullptr
			const Tuile* ptrTuileVoisine = envJ.getTuile(posVoisine);
			const Tuile& tuileVoisine = *ptrTuileVoisine;

			if (!tuileVoisine.JetonFaunePresent()) continue;
			// incrementer le nb de cette faune, si elle n'existe pas elle sera crée dans FaunesAdj
			FaunesAdj[tuileVoisine.getFaunePlace()]++;
		}
		for (const auto& [faune,nb]: FaunesAdj) {
			if (nb == 2) nbPaires++;
		}
		PairesAdj.push_back(nbPaires);
	}
	for (int renard : PairesAdj) {
		switch (renard) {
		case 1: scoreTotal += 3; break;
		case 2: scoreTotal += 5; break;
		case 3: scoreTotal += 7; break;
		default: break;
		}
	}
	return scoreTotal;
}

// Renard C

int CarteRenardC::CalculScore(const EnvJoueur& envJ) const
{
	const auto& mapPositionsJetons = envJ.getMapPositionsJetons();
	if (mapPositionsJetons.count(Faune::renard) == 0) return 0; // pas de renard

	const auto& positionsRenard = mapPositionsJetons.at(Faune::renard);
	const auto& carte = envJ.getTuiles();

	int scoreTotal = 0;

	for (const Position& position : positionsRenard) {
		std::unordered_map<Faune, int> faunesAdj;

		// Parcours des cases adjacentes
		for (const Position& posVoisine : position.getVecteurPositionsAdjacentes()) {
			if (!envJ.aTuileConfirmee(posVoisine)) continue;

			const Tuile* tuileVoisine = envJ.getTuile(posVoisine);
			if (!tuileVoisine->JetonFaunePresent()) continue;

			Faune faune = tuileVoisine->getFaunePlace();
			if (faune != Faune::renard) {
				faunesAdj[faune]++;
			}
		}

		// Trouver le max
		int maxAdj = 0;
		for (const auto& [faune, nb] : faunesAdj) {
			if (nb > maxAdj) maxAdj = nb;
		}

		scoreTotal += maxAdj;
	}

	return scoreTotal;
}

// Wapiti A 

int CarteWapitiA::CalculScore(const EnvJoueur& envJ) const
{
	std::unordered_set<Position> PositionsVisitees;
	std::vector<int> taillesChaines;
	int scoreTotal = 0;

	const auto& carte = envJ.getTuiles();

	for (const auto& [position, tuile] : carte) {
		// on ignore la tuile si déjà visitée
		if (PositionsVisitees.count(position) == 1) continue;
		// on ignore la tuile si pas de jeton faune ou jeton non wapiti
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::wapiti) continue;

		int tailleChaine = explorerChaine(carte, position, PositionsVisitees, std::nullopt);

		if (tailleChaine > 0) {
			taillesChaines.push_back(tailleChaine);
		}
	}

	for (int chaine : taillesChaines) {
		switch (chaine) {
		case 0:  scoreTotal += 0;  break;
		case 1:  scoreTotal += 2;  break;
		case 2:  scoreTotal += 5;  break;
		case 3:  scoreTotal += 9;  break;
		default: scoreTotal += 13; break;  // 3 +
		}
	}

	return scoreTotal;
}

int CarteWapitiA::explorerChaine(const std::unordered_map<Position, Tuile>& carte, const Position& position, std::unordered_set<Position>& PositionsVisitees, std::optional<Direction> direction) const
{
	if (PositionsVisitees.count(position)) return 0;
	if (carte.count(position) == 0) return 0;

	const Tuile& tuile = carte.at(position);
	if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::wapiti) return 0;

	PositionsVisitees.insert(position);

	// cas 0: on cherche une direction et on explore dans les 2 sens
	if (!direction.has_value()) {
		// itération sur les positions adjacentes à 'position'
		for (const Position& posVoisine : position.getVecteurPositionsAdjacentes()) {
			if (carte.count(posVoisine) == 0) continue;

			const Tuile& tuileVoisine = carte.at(posVoisine);
			if (!tuileVoisine.JetonFaunePresent() || tuileVoisine.getFaunePlace() != Faune::wapiti) continue;

			//trouvé un wapiti voisin
			Direction dir = coteTangent(position, posVoisine);
			Direction opp = getDirectionOpposee(dir);

			int taille1 = explorerChaine(carte, posVoisine, PositionsVisitees, dir);
			int taille2 = explorerChaine(carte, position.getPositionAdjacente(opp), PositionsVisitees, opp);

			return 1 + taille1 + taille2; //1 : la pos actuelle compte + chaine dans une direction + chaine direction opposee
		}
		//pas de voisin wapiti, alors juste la pos actuelle
		return 1;
	}

	//else, on a déjà la direction, on continue à la parcourir
	Position suivante = position.getPositionAdjacente(direction.value());
	return 1 + explorerChaine(carte, suivante, PositionsVisitees, direction);
}

// Wapiti B

int CarteWapitiB::CalculScore(const EnvJoueur& envJ) const
{
	const auto& mapPositionsJetons = envJ.getMapPositionsJetons();
	if (mapPositionsJetons.count(Faune::wapiti) == 0) return 0; // pas de renard

	const auto& positionsWapiti = mapPositionsJetons.at(Faune::wapiti);
	int nbF1 = 0, nbF2 = 0, nbF3 = 0, nbF4 = 0;
	int scoreTotal;
	std::unordered_set<Position> wapitisVisites;

	for (const Position& position : positionsWapiti) {
		std::vector<Position> wapitisAdj;
		if (wapitisVisites.count(position)) continue;

		for (const Position& posVoisine : position.getVecteurPositionsAdjacentes()) {

			if (positionsWapiti.count(posVoisine) && !wapitisVisites.count(posVoisine)) {
				wapitisAdj.push_back(posVoisine);
			}
		}

		if (wapitisAdj.empty()) {
			nbF1++;
			wapitisVisites.insert(position);
		}
		if (wapitisAdj.size() == 1) {
			int nbAdj = 0;
			for (const Position& posVoisine : wapitisAdj[0].getVecteurPositionsAdjacentes()) {
				if (positionsWapiti.count(posVoisine)) {
					nbAdj++;
				}
			}
			if (nbAdj == 1) {
				nbF2++;
				wapitisVisites.insert(position);
				wapitisVisites.insert(wapitisAdj[0]);
			}
			else {
				nbF1++;
				wapitisVisites.insert(position);
			}
		}
		if (wapitisAdj.size() == 2 &&
			wapitisAdj[0].estAdjacente(wapitisAdj[1]) &&
			!wapitisVisites.count(wapitisAdj[0]) &&
			!wapitisVisites.count(wapitisAdj[1])) {

			int nbAdj = 0;
			for (const Position& posVoisine : wapitisAdj[0].getVecteurPositionsAdjacentes()) {
				if (positionsWapiti.count(posVoisine)) {
					nbAdj++;
				}
			}
			if (nbAdj == 2) { //c'est bien un triangle
				nbF3++;
				wapitisVisites.insert(position);
				wapitisVisites.insert(wapitisAdj[0]);
				wapitisVisites.insert(wapitisAdj[1]);
				continue;
			}

		}
		else if (wapitisAdj.size() == 3) {
			//on récupère les 3 possibilités d'adjacence d'après les indices des positions dans le vecteur
			bool possibilite1 = (wapitisAdj[0].estAdjacente(wapitisAdj[1]) && wapitisAdj[1].estAdjacente(wapitisAdj[2]));
			bool possibilite2 = (wapitisAdj[0].estAdjacente(wapitisAdj[2]) && wapitisAdj[1].estAdjacente(wapitisAdj[2]));
			bool possibilite3 = (wapitisAdj[0].estAdjacente(wapitisAdj[1]) && wapitisAdj[0].estAdjacente(wapitisAdj[2]));
			bool dejaVisite = (wapitisVisites.count(wapitisAdj[0]) || wapitisVisites.count(wapitisAdj[1]) || wapitisVisites.count(wapitisAdj[2]));
			if ((possibilite1 || possibilite2 || possibilite3) && !dejaVisite) {
				nbF4++;
				wapitisVisites.insert(position);
				wapitisVisites.insert(wapitisAdj[0]);
				wapitisVisites.insert(wapitisAdj[1]);
				wapitisVisites.insert(wapitisAdj[2]);
			}
		}
	}

		/*
		else if (wapitisAdj.size() == 2) {

			if (!wapitisAdj[0].estAdjacente(wapitisAdj[1])) continue;

			int q = position.getQ();
			int r = position.getR();
			int s = position.getS();

			// on recupere les 2 positions directement au-dessus et en dessous
			Position posPossible1 = Position(q + 1, r - 2, s + 1);
			Position posPossible2 = Position(q - 1, r + 2, s - 1);

			// on s'assure qu'elles sont wapiti et que l'adjacence est correcte pour la forme 4 
			bool possibilite1 = positionsWapiti.count(posPossible1) && wapitisAdj[0].estAdjacente(posPossible1) && wapitisAdj[1].estAdjacente(posPossible1);
			bool possibilite2 = positionsWapiti.count(posPossible2) && wapitisAdj[0].estAdjacente(posPossible2) && wapitisAdj[1].estAdjacente(posPossible2);

			if (possibilite1) {
				//wapitisVisites.insert(posPossible1);
				nbF4++;
			}
			else if (possibilite2) {
				//wapitisVisites.insert(posPossible2);
				nbF4++;
			}

			// la forme 4 n'est pas validée, donc il s'agit d'un triangle de wapitis
			else {
				nbF3++;
			}
			continue;
		}
		else if (wapitisAdj.size() == 3) {
			
			//on récupère les 3 possibilités d'adjacence d'après les indices des positions dans le vecteur
			bool possibilite1 = (wapitisAdj[0].estAdjacente(wapitisAdj[1]) && wapitisAdj[1].estAdjacente(wapitisAdj[2]));
			bool possibilite2 = (wapitisAdj[0].estAdjacente(wapitisAdj[2]) && wapitisAdj[1].estAdjacente(wapitisAdj[2]));
			bool possibilite3 = (wapitisAdj[0].estAdjacente(wapitisAdj[1]) && wapitisAdj[0].estAdjacente(wapitisAdj[2]));
			if (possibilite1 || possibilite2 || possibilite3) nbF4++;
		}
		
	}
	*/
	/*
	std::cout << "\n nbf1 :" << nbF1;
	std::cout << "\n nbf2 :" << nbF2;
	std::cout << "\n nbf3 :" << nbF3;
	std::cout << "\n nbf4 :" << nbF4;
	std::cout <<"\n total :"<< nbF1 * 2 + nbF2 * 5 + nbF3 * 9 + nbF4 * 13<<"\n";
	*/
	return (scoreTotal = nbF1 * 2 + nbF2 * 5 + nbF3 * 9 + nbF4 * 13);
}

// Wapiti C

int CarteWapitiC::CalculScore(const EnvJoueur& envJ) const
{
	const auto& mapPositionsJetons = envJ.getMapPositionsJetons();
	if (mapPositionsJetons.count(Faune::wapiti) == 0) return 0;

	auto positionsWapiti = mapPositionsJetons.at(Faune::wapiti);
	std::unordered_set<Position> nonVisites = positionsWapiti;
	int scoreTotal = 0;

	while (!nonVisites.empty()) {
		// nouveau groupe
		Position start = *nonVisites.begin();
		std::stack<Position> pile;
		pile.push(start);
		nonVisites.erase(start);

		int tailleGroupe = 0;

		while (!pile.empty()) {
			Position current = pile.top();
			pile.pop();
			tailleGroupe++;

			for (const Position& adj : current.getVecteurPositionsAdjacentes()) {
				if (nonVisites.count(adj)) {
					pile.push(adj);
					nonVisites.erase(adj);
				}
			}
		}
		switch (tailleGroupe) {
		case 1: scoreTotal += 2; break;
		case 2: scoreTotal += 4; break;
		case 3: scoreTotal += 7; break;
		case 4: scoreTotal += 10; break;
		case 5: scoreTotal += 14; break;
		case 6: scoreTotal += 18; break;
		case 7: scoreTotal += 23; break;
		default: scoreTotal += 28; break; //8+
		}
	}

	return scoreTotal;
}

// Variante Familiale

int VarianteFamiliale::CalculScore(const EnvJoueur& envJ) const
{
	const auto& mapPositionsJetons = envJ.getMapPositionsJetons();
	int scoreTotal = 0;

	for (const auto& [faune, positions] : mapPositionsJetons) {
		std::unordered_set<Position> nonVisites = positions;

		while (!nonVisites.empty()) {
			Position posDepart = *nonVisites.begin(); //recuperer la premiere position  non visitee
			std::stack<Position> pile;
			pile.push(posDepart);
			nonVisites.erase(posDepart);

			int tailleGroupe = 0;

			while (!pile.empty()) {
				Position posCourante = pile.top();
				pile.pop();
				tailleGroupe++;

				for (const Position& posVoisine : posCourante.getVecteurPositionsAdjacentes()) {
					if (nonVisites.count(posVoisine)) {
						pile.push(posVoisine);
						nonVisites.erase(posVoisine);
					}
				}
			}

			// Scoring identique pour toutes les faunes
			switch (tailleGroupe) {
			case 1: scoreTotal += 2; break;
			case 2: scoreTotal += 5; break;
			default: scoreTotal += 9; break;
			}
		}
	}

	return scoreTotal;
}

int VarianteIntermediaire::CalculScore(const EnvJoueur& envJ) const
{
	const auto& mapPositionsJetons = envJ.getMapPositionsJetons();
	int scoreTotal = 0;

	for (const auto& [faune, positions] : mapPositionsJetons) {
		std::unordered_set<Position> nonVisites = positions;

		while (!nonVisites.empty()) {
			Position posDepart = *nonVisites.begin(); //recuperer la premiere position  non visitee
			std::stack<Position> pile;
			pile.push(posDepart);
			nonVisites.erase(posDepart);

			int tailleGroupe = 0;

			while (!pile.empty()) {
				Position posCourante = pile.top();
				pile.pop();
				tailleGroupe++;

				for (const Position& posVoisine : posCourante.getVecteurPositionsAdjacentes()) {
					if (nonVisites.count(posVoisine)) {
						pile.push(posVoisine);
						nonVisites.erase(posVoisine);
					}
				}
			}

			// Scoring identique pour toutes les faunes
			switch (tailleGroupe) {
			case 0: break;
			case 1: break;
			case 2: scoreTotal += 5; break;
			case 3: scoreTotal += 8; break;
			default: scoreTotal += 12; break; //4+
			}
		}
	}

	return scoreTotal;
}
