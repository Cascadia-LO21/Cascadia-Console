#include "score.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include "enums.h"
#include "position.h"


std::array<std::vector<std::vector<Position>>, 5> EvalScore::ScoreHabitat::repartitionZone(const EnvJoueur& joueur)
{
	const std::unordered_map<Position, Tuile>& carte = joueur.getTuiles();
	std::unordered_map<Position, std::array<bool, 6>> carteAParcourir = parcourirCarte(carte);
	std::array<std::vector<std::vector<Position>>, 5> carteIndexee;
	std::vector<Position> positionsVisitees;
	for (const auto& [position, directions] : carteAParcourir) {
		if (std::find(positionsVisitees.begin(), positionsVisitees.end(), position) != positionsVisitees.end()) {
			// appartenance, alors skip iteration
		}
		else {
			// cas ou on n'a pas parcouru les 6 directions de la Tuile
			int currentIndice = getFirstFalseIndex(directions);
			while (currentIndice != -1) {
				const Tuile& currentTuile = carte.at(position);
				Habitat currentHabitat = currentTuile.getHabitats()[currentIndice];
				// MAJ carteIndexee du currentTuile
				size_t currentIndice = carteIndexee[static_cast<int>(currentHabitat)].size() + 1;
				carteIndexee[static_cast<int>(currentHabitat)].resize(currentIndice);
				parcourHabitat(position, currentHabitat, carte, positionsVisitees, carteAParcourir, carteIndexee[static_cast<int>(currentHabitat)][currentIndice]);
			}
			// fini de parcourir une tuile
			positionsVisitees.push_back(position);
		}
	}
}

void EvalScore::ScoreHabitat::updateTailleMaxTous(int indice, std::array<std::vector<std::vector<Position>>, 5> carteIndexee)
{
	auto maxRetenu = getTailleMaxTous();
	auto maxJoueur = maxRetenu[indice];
	int courantIndice = 0;
	for (auto valeurMax : maxJoueur) {
		valeurMax = retournerMax(carteIndexee, courantIndice);
		courantIndice++;
	}
}

void EvalScore::ScoreHabitat::updateBonusTous(int nbJoueurs)
{
	// ref tableau du Rapport 1
	auto bonus = getBonusTous();
	auto maxRetenu = getTailleMaxTous();

	if (nbJoueurs == 1) {
		for (int i = 0; i < 5; i++) {
			if (maxRetenu[0][i] >= 7) {
				bonus[0][i] = 2;
			}
		}
	}
	else if (nbJoueurs == 2) {
		// chaque type faune
		for (int i = 0; i < 5; i++) {
			// V rifier le joueur 1
			if (maxRetenu[0][i] > maxRetenu[1][i]) {
				bonus[0][i] = 2; // 1er joueur
			}
			else if (maxRetenu[0][i] < maxRetenu[1][i]) {
				bonus[1][i] = 2; // 1er joueur
			}
			else {
				bonus[0][i] = 1; //  galit 
				bonus[1][i] = 1;
			}
		}
	}
	else {
		// 3 et 4 joueurs
		int valPremier = 0;
		int valDeuxieme = 0;
		std::vector<int> ensemblePremiers;
		std::vector<int> ensembleDeuxiemes;

		for (int i = 0; i < 5; i++) {
			// Trouver le maximum et le deuxi me maximum
			for (int j = 0; j < nbJoueurs; j++) {
				if (maxRetenu[j][i] > valPremier) {
					valDeuxieme = valPremier;
					ensembleDeuxiemes = ensemblePremiers;
					valPremier = maxRetenu[j][i];
					ensemblePremiers = { j }; // R initialiser avec le joueur courant
				}
				else if (maxRetenu[j][i] == valPremier) {
					ensemblePremiers.push_back(j); // Ajouter   l'ensemble des premiers
				}
				else if (maxRetenu[j][i] > valDeuxieme && maxRetenu[j][i] < valPremier) {
					valDeuxieme = maxRetenu[j][i];
					ensembleDeuxiemes = { j }; // R initialiser avec le joueur courant
				}
				else if (maxRetenu[j][i] == valDeuxieme) {
					ensembleDeuxiemes.push_back(j); // Ajouter   l'ensemble des deuxi mes
				}
			}

			// Attribuer les bonus
			if (ensemblePremiers.size() > 1) {
				//  galit  pour le premier
				for (int joueur : ensemblePremiers) {
					bonus[joueur][i] = 2; // +2 chacun
				}
			}
			else {
				// 1er joueur
				for (int joueur : ensemblePremiers) {
					bonus[joueur][i] = 3; // +3 pour le premier
				}
			}

			// Attribuer le bonus pour le deuxi me plus  tendu
			if (ensembleDeuxiemes.size() > 1) {
				//  galit  pour le deuxi me, rien   attribuer
				for (int joueur : ensembleDeuxiemes) {
					continue;
					// bonus[joueur][i] = 0; // 0 pour  galit 
				}
			}
			else {
				// 2e joueur
				for (int joueur : ensembleDeuxiemes) {
					bonus[joueur][i] = 1; // +1 pour le deuxi me
				}
			}
		}
	}
}

void EvalScore::ScoreHabitat::updateScore(int indice, EnvJoueur& joueur)
{
	auto scoreHabitat = joueur.getScoreHabitat(); 
	auto bonus = getBonusTous();
	auto maxRetenu = getTailleMaxTous();
	for (int i = 0; i < 5; i++) {
		scoreHabitat[i] = maxRetenu[indice][i] + bonus[indice][i];
	}
	return;
}

void EvalScore::ScoreHabitat::parcourHabitat(Position position, Habitat currentHabitat, const std::unordered_map<Position, Tuile>& carte, std::vector<Position> positionDirectionVisitees, std::unordered_map<Position, std::array<bool, 6>> carteAParcourir, std::vector<Position> zone)
{
	zone.push_back(position);
	const std::array<Habitat, 6> ensemble = carte.at(position).getHabitats();
	// dans indiceMemeHabitat on a deja fait la verification si deja visite ou non
	std::vector<int> indicesAParcourir = indiceMemeHabitat(currentHabitat, ensemble, carteAParcourir.at(position));
	for (auto currentIndice : indicesAParcourir) {
		const Position voisin = position.getPositionAdjacente(static_cast<Direction>(currentIndice));
		// verif si voisin existe dans carte
		if (carte.find(voisin) != carte.end()) {
			// verif si deja visitee
			if (std::find(positionDirectionVisitees.begin(), positionDirectionVisitees.end(), voisin) == positionDirectionVisitees.end()) {
				// verif voisin
				Direction directionVoisin = getDirectionOpposee(static_cast<Direction> (currentIndice));
				Habitat voisinHabitat = carte.at(voisin).getHabitats()[static_cast<int>(directionVoisin)];
				// verif si (dans voisin) cote adjacent est meme habitat avec Direction getDirectionOpposee(Direction dir);
				if (voisinHabitat != currentHabitat) {
					// non = fin de parcours dans cette direction
					continue;
				}
				else {
					// oui
					parcourHabitat(voisin, currentHabitat, carte, positionDirectionVisitees, carteAParcourir, zone);
				}
			}
		}
		// MAJ currentIndice parcouru
		carteAParcourir.at(position)[currentIndice] = true;
	}
	return;
}

std::unordered_map<Position, std::array<bool, 6>> EvalScore::ScoreHabitat::parcourirCarte(const std::unordered_map<Position, Tuile>& carte)
{
	std::unordered_map<Position, std::array<bool, 6>> carteAParcourir;

	for (const auto& paire : carte) {
		carteAParcourir[paire.first] = std::array<bool, 6>{ false, false, false, false, false, false };// pour les 6 directions
	}

	return carteAParcourir;
}

int EvalScore::ScoreHabitat::getFirstFalseIndex(const std::array<bool, 6>& directions)
{
	for (int i = 0; i < directions.size(); ++i) {
		if (!directions[i]) {
			return i;
		}
	}
	return -1;
}

std::vector<int> EvalScore::ScoreHabitat::indiceMemeHabitat(Habitat habitat, const std::array<Habitat, 6> ensemble, std::array<bool, 6> dejaVisite)
{
	std::vector<int> vecteurIndice;
	for (int i = 0; i < ensemble.size(); i++) {
		if (!dejaVisite[i] && ensemble[i] == habitat) {
			vecteurIndice.push_back(i);
		}
	}
	return vecteurIndice;
}

int EvalScore::ScoreHabitat::retournerMax(std::array<std::vector<std::vector<Position>>, 5> carteIndexee, int indice) {
	if (indice < 0 || indice >= carteIndexee.size()) {
		throw std::out_of_range("Indice hors limites.");
	}

	int courantHabitatMax = 0;
	for (const auto& ensembleZones : carteIndexee[indice]) {
		int nouveauTaille = static_cast<unsigned int>(ensembleZones.size());
		if (nouveauTaille > courantHabitatMax) {
			courantHabitatMax = nouveauTaille;
		}
	}
	return courantHabitatMax;
}

// Attention: cette fonction ne mobilise que la carte marquage A
void EvalScore::calculScoreFaune(Partie& partie)
{
	std::vector<std::unique_ptr<CarteMarquage>> cartes;
	cartes.push_back(std::make_unique<CarteSaumon>());
	cartes.push_back(std::make_unique<CarteOurs>());
	cartes.push_back(std::make_unique<CarteBuse>());
	cartes.push_back(std::make_unique<CarteRenard>());
	cartes.push_back(std::make_unique<CarteWapiti>());

	int nbJoueur = partie.getNbJoueurs();
	auto& joueursModifiable = partie.getJoueursModifiable();
	// parcours faune
	for (auto& joueur : joueursModifiable) {
		int i = 0;
		for (const auto& carteMarquage : cartes) {
			joueur.getScoreFaune()[i] = carteMarquage->methodeCalculA(joueur.getTuiles());
			i++;
		}
	}
	return;
}

void EvalScore::ScoreHabitat::caclulScoreHabitat(Partie& partie)
{
	// ajustement de la taille
	int nbJoueur = partie.getNbJoueurs();
	setTailleMaxTous(nbJoueur);
	setBonusTous(nbJoueur);

	const auto& joueurs = partie.getJoueurs();
	for (int i = 0; i < nbJoueur; i++) {
		updateTailleMaxTous(i, repartitionZone(joueurs[i]));
	}

	auto& joueursModifiable = partie.getJoueursModifiable();
	updateBonusTous(nbJoueur);
	for (int i = 0; i < nbJoueur; i++) {
		updateScore(i, joueursModifiable[i]);
	}
}



int EvalScore::CarteOurs::methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const {

	std::unordered_set<Position> PositionsVisitees;
	int nbPairesOurs = 0;

	//parcourt des tuiles de carte
	for (const auto& [position, tuile] : carte) { // [cle,valeur] appartenant   carte

		//on ignore la tuile si deja visit e
		if (PositionsVisitees.count(position) == 1) continue;
		//on ignre la tuile si pas de jeton faune ou jeton non ours, et on la marque comme visitee
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::ours) continue;

		//on v rifie s'il y a d'autres ours adjacents
		int nbOursAdj = 0;
		Position posOursAdjUnique; //sera instanci  plus tard s'il y a un ours adjacent

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

		//on v rifie si l'unique ours adj   la position courante n'est pas adjacent   un autre ours
		if (nbOursAdj == 1) {
			int nbOursAdj2 = 0;
			std::vector<Position> vec = posOursAdjUnique.getVecteurPositionsAdjacentes();
			for (unsigned int i = 0; i < 6; i++) {

				Position posVoisine = vec[i];
				if (carte.count(posVoisine) == 1) { //il ne faut pas v rifier si elle a d j   t  visit e
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


int EvalScore::CarteBuse::methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const {
	//initialisations
	std::unordered_set<Position> PositionsVisitees;
	int nbBusesIsolees = 0;

	//parcourt des tuiles de carte
	for (const auto& [position, tuile] : carte) { // [cle,valeur] appartenant   carte

		//on ignore la tuile si deja visit e
		if (PositionsVisitees.count(position) == 1) continue;
		//on ignre la tuile si pas de jeton faune ou jeton non buse
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::buse) continue;

		//on v rifie s'il y a d'autres buses adjacentes
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


int EvalScore::CarteRenard::methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const
{
	int scoreTotal = 0;
	std::unordered_set<Position> PositionsVisitees;
	std::vector<int> AdjRenards;

	//parcourt des tuiles de carte
	for (const auto& [position, tuile] : carte) {
		std::unordered_set<Faune> FaunesAdjacentes; //afin de stocker les faunes sans duplication, puis avoir la taille

		//on ignore la tuile si deja visit e
		if (PositionsVisitees.count(position) == 1) continue;
		//on ignre la tuile si pas de jeton faune ou jeton non renard
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::renard) continue;

		//parcourir les tuiles adjacentes   la tuile renard
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
		//on compte le nombre de types adj pour chaque renard et on l'ajoute   AdjRenards
		AdjRenards.push_back(static_cast<int>(FaunesAdjacentes.size()));
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


int EvalScore::CarteSaumon::methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const
{
	std::unordered_set<Position> PositionsVisitees;
	std::vector<int> taillesChaines;
	int scoreTotal = 0;

	for (const auto& [position, tuile] : carte) {
		int longueur_chaine = 0;

		//on ignore la tuile si deja visit e
		if (PositionsVisitees.count(position) == 1) continue;
		//on ignore la tuile si pas de jeton faune ou jeton non saumon
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::saumon) continue;


		//position actuelle contient saumon
		int tailleChaine = explorerChaineSaumonA(carte, position, PositionsVisitees, nullptr); //nullptr car premier element n'a pas de pere
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

int EvalScore::CarteSaumon::explorerChaineSaumonA(const std::unordered_map<Position, Tuile>& carte, const Position& position, std::unordered_set<Position>& PositionsVisitees, const Position* pos_pere) const
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
		if (nbSaumonsAdj > 2) return 0; //chaine annul e
		if (PositionsVisitees.count(posVoisine) == 0) {
			//appel r cursif sur la position voisine, et la position actuelle est l'argument pere
			int tailleSuite = explorerChaineSaumonA(carte, posVoisine, PositionsVisitees, &position);
			if (tailleSuite == 0) return 0; //la chaine est invalide, donc on arrete
			//else
			taille += tailleSuite;
		}
	}

	return taille;
}

int EvalScore::CarteWapiti::methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const
{
	std::unordered_set<Position> PositionsVisitees;
	std::vector<int> taillesChaines;
	int scoreTotal = 0;

	for (const auto& [position, tuile] : carte) {
		int longueur_chaine = 0;

		//on ignore la tuile si deja visit e
		if (PositionsVisitees.count(position) == 1) continue;
		//on ignore la tuile si pas de jeton faune ou jeton non wapiti
		if (!tuile.JetonFaunePresent() || tuile.getFaunePlace() != Faune::wapiti) continue;

		//position actuelle contient saumon
		int tailleChaine = explorerChaineWapitiA(carte, position, PositionsVisitees, std::nullopt); //nullopt car on n'a pas encore trouv  la direction de la chaine
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

int EvalScore::CarteWapiti::explorerChaineWapitiA(const std::unordered_map<Position, Tuile>& carte, const Position& position, std::unordered_set<Position>& PositionsVisitees, std::optional<Direction> direction) const
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

			//trouv  un wapiti voisin
			Direction dir = coteTangent(position, posVoisine);
			Direction opp = getDirectionOpposee(dir);

			int taille1 = explorerChaineWapitiA(carte, posVoisine, PositionsVisitees, dir);
			int taille2 = explorerChaineWapitiA(carte, position.getPositionAdjacente(opp), PositionsVisitees, opp);

			return 1 + taille1 + taille2; //1 : la pos actuelle compte + chaine dans une direction + chaine direction opposee
		}
		//pas de voisin wapiti, alors juste la pos actuelle
		return 1;
	}

	//else, on a d j  la direction, on continue   la parcourir
	Position suivante = position.getPositionAdjacente(direction.value());
	return 1 + explorerChaineWapitiA(carte, suivante, PositionsVisitees, direction);

}
