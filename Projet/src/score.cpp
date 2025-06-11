#include <queue>
#include <unordered_set>
#include <algorithm>
#include <exception>
#include "score.h"
#include "carte_marquage.h"

// appliquer toutes les cartes marquage X sur le player, et ces resultats seront stockes dans un ScoreJoueur
void Score::calculFauneLettre(const EnvJoueur& player, ScoreJoueur& sj, char lettre) {
	std::vector<Faune> faunes = { Faune::buse, Faune::ours, Faune::renard, Faune::saumon, Faune::wapiti };
	for (auto f : faunes) {
		std::string nomCarte = fauneToString(f) + lettre; // exemple : "saumonA"
		auto carte = CarteMarquageStandardFactory::creerCarte(nomCarte);
		USI points = carte->CalculScore(player);
		sj.pointsFaunes[f] = points;
	}
}

void Score::calculFauneVariante(const EnvJoueur& player, ScoreJoueur& sj, std::string variante) {
	auto carteVar = CarteMarquageVarianteFactory::creerCarte(variante);
	sj.pointsFaunes = carteVar->CalculScore(player);
}

// calcule la plus grande zone dans le plateau du joueur pour chaque type d'Habitat
void Score::ScoreFeuille::calculPointsHabitats(const EnvJoueur& player, ScoreJoueur& sj) {
	const std::unordered_map<Position, Tuile>& plateau = player.getTuiles();
	std::vector<Habitat> habitats = { Habitat::fleuve, Habitat::foret, Habitat::marais, Habitat::montagne, Habitat::prairie };

	for (auto h : habitats) {
		std::unordered_set<Position> posVisitees;
		USI zoneMax = 0;

		for (const auto& [pos, tuile] : plateau) {
			if (posVisitees.count(pos)) 
				continue; // si deja visitee

			if (std::find(tuile.getHabitats().begin(), tuile.getHabitats().end(), h) == tuile.getHabitats().end()) 
				continue; // si la tuile ne possede pas l'habitat en question

			// algo de recherche en largeur (BFS), implementee avec une file (FIFO)
			// pour decouvrir les tuiles adjacentes a celle en question d'abord
			std::queue<Position> file; // les Positions a visiter
			std::unordered_set<Position> zone; 
			file.push(pos);
			zone.insert(pos);
			//posVisitees.insert(pos);

			while (!file.empty()) { // tant qu'il reste des positions a visiter
				Position current = file.front();
				file.pop();
				posVisitees.insert(current);

				const Tuile& t = *player.getTuile(current);
				const auto& vecHabitats = t.getHabitats();

				for (USI i = 0; i < t.getHabitats().size(); i++) {
					if (vecHabitats[i] != h) continue;

					// si sur ce cote i il y a l'habitat h, on examine si la tuile adjacente par ici existe et possede l'habitat en question
					Position posAdj = current.getPositionAdjacente(static_cast<Direction>(i));
					const Tuile* tAdj = player.getTuile(posAdj); 
					if (!tAdj) continue; // pas de tuile a cette position
					if (zone.count(posAdj)) continue; // deja dans la zone
					if (tAdj->getHabitats()[(i+3)%6] != h) continue; // si le cote en question n'est pas de type h
					
					if (!posVisitees.count(posAdj)) {
						file.push(posAdj);
						zone.insert(posAdj);
					}
				}
			}
			if (zone.size() > zoneMax) zoneMax = static_cast<USI>(zone.size());
		}
		sj.pointsHabitats[h] = zoneMax; // stocker la plus grande taille de la zone h trouvee
	}
}

void Score::ScoreFeuille::calculerBonusHabitats(const std::vector<EnvJoueur>& players) {
	if (scores.size() != players.size()) throw std::invalid_argument("Il y a plus de joueurs que de scores calcules !");
	
	if (scores.size() == 1) { // si jeu solitaire
		auto& scoreJoueur = scores[players[0].getPseudo()];
		for (const auto& [hab, taille] : scoreJoueur.pointsHabitats) {
			if (taille >= 7)
				scoreJoueur.pointsHabitats[hab] = 2;
		}
	}

	else { // si 2+ joueurs

	}
}
 