#include <queue>
#include <unordered_set>
#include <algorithm>
#include <exception>
#include <iostream>
#include "score.h"
#include "carte_marquage.h"
#include "enums.h"
#include "affichage.h"

// appliquer toutes les cartes marquage X sur le player, et ces resultats seront stockes dans un ScoreJoueur
void Score::calculFauneLettre(const EnvJoueur& player, ScoreJoueur& sj, std::string lettre) {
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
	std::vector<Habitat> habitats = { Habitat::fleuve, Habitat::foret, Habitat::marais, Habitat::montagne, Habitat::prairie };
	USI nbJoueurs = static_cast<USI>(scores.size());

	if (nbJoueurs == 1) { // si jeu solitaire
		auto& scoreJoueur = scores[players[0].getPseudo()];
		for (const auto& [hab, taille] : scoreJoueur.pointsHabitats) {
			if (taille >= 7)
				scoreJoueur.pointsHabitatsBonus[hab] = 2;
			else
				scoreJoueur.pointsHabitatsBonus[hab] = 0;
		}
	}

	else { // si 2+ joueurs
		for (auto h: habitats) {

			// tailles pour chaque joueur
			std::vector<int> tailles;
			for (const auto& player : players) {
				tailles.push_back(scores.at(player.getPseudo()).pointsHabitats.at(h));
			}

			for (const auto& player : players) {
				scores[player.getPseudo()].pointsHabitatsBonus[h] = 0;
			}

			if (players.size() == 2) { // 2 joueurs
				if (tailles[0] > tailles[1]) {
					scores[players[0].getPseudo()].pointsHabitatsBonus[h] = 2;
				}
				else if (tailles[0] < tailles[1]) {
					scores[players[1].getPseudo()].pointsHabitatsBonus[h] = 2;
				}
				else {
					scores[players[0].getPseudo()].pointsHabitatsBonus[h] = 1;
					scores[players[1].getPseudo()].pointsHabitatsBonus[h] = 1;
				}
			}
			else { // 3 ou 4 joueurs
				int valPremier = -1, valDeuxieme = -1;
				std::vector<int> ensemblePremiers, ensembleDeuxiemes;

				// Trouver les premiers et deuxièmes
				for (int j = 0; j < (int)players.size(); ++j) {
					if (tailles[j] > valPremier) {
						valDeuxieme = valPremier;
						ensembleDeuxiemes = ensemblePremiers;
						valPremier = tailles[j];
						ensemblePremiers = { j };
					}
					else if (tailles[j] == valPremier) {
						ensemblePremiers.push_back(j);
					}
					else if (tailles[j] > valDeuxieme && tailles[j] < valPremier) {
						valDeuxieme = tailles[j];
						ensembleDeuxiemes = { j };
					}
					else if (tailles[j] == valDeuxieme) {
						ensembleDeuxiemes.push_back(j);
					}
				}

				// Attribution des bonus pour les premiers
				if (ensemblePremiers.size() == 1) {
					scores[players[ensemblePremiers[0]].getPseudo()].pointsHabitatsBonus[h] = 3;
				}
				else if (ensemblePremiers.size() == 2) {
					for (int idx : ensemblePremiers)
						scores[players[idx].getPseudo()].pointsHabitatsBonus[h] = 2;
				}
				else if (ensemblePremiers.size() == 3 || ensemblePremiers.size() == 4) {
					for (int idx : ensemblePremiers)
						scores[players[idx].getPseudo()].pointsHabitatsBonus[h] = 1;
				}

				// Attribution des bonus pour les deuxièmes
				if (ensemblePremiers.size() == 1) {
					if (ensembleDeuxiemes.size() == 1) {
						scores[players[ensembleDeuxiemes[0]].getPseudo()].pointsHabitatsBonus[h] = 1;
					}
					// Si égalité à la 2e place, pas de points
				}
				// Si égalité au 1er, pas de points pour les deuxièmes
			}
		}
	}

}

void Score::ScoreFeuille::calculScoresPartie(const Partie& p) {
	scores.clear();
	const std::vector<EnvJoueur> players = p.getJoueurs();
	if (p.getVariante() == Variante::standard)
		setStrategieFaune(std::make_unique<CalculScoreFauneStandard>(marquageToString(p.getMarquage())));
	else // si variante famille ou intermediaire ou autre
		setStrategieFaune(std::make_unique<CalculScoreFauneVariante>(varianteToString(p.getVariante())));

	for (const auto& player : players) {
		ScoreJoueur sj;
		strategieFaune->calculPointsFaunes(player, sj);
		calculPointsHabitats(player, sj);
		
		calculTotalFaunes(sj);
		//calculTotalHabitats(sj);
		sj.nbJetonsNature = player.getNbJetonsNature();
		scores[player.getPseudo()] = sj;
	}

	calculerBonusHabitats(players);
	for(const auto& player:players) calculTotalHabitats(scores[player.getPseudo()]);
	
	for (const auto& player : players) {
		calculTotalFinal(scores[player.getPseudo()]);
	}
}


std::vector<std::string> Score::ScoreFeuille::getGagnants() const {
	std::vector<std::string> gagnants;
	int maxScore = -1;

	// trouver le score total maximum
	for (const auto& [pseudo, sj] : scores) {
		if (static_cast<int>(sj.totalFinal) > maxScore) {
			maxScore = sj.totalFinal;
			gagnants = { pseudo };
		}
		else if (sj.totalFinal == maxScore) {
			gagnants.push_back(pseudo);
		}
	}

	if (gagnants.size() == 1) return gagnants;

	// 3. Départage avec les jetons nature
	int maxJetons = -1;
	std::vector<std::string> gagnantsJetons;
	for (const auto& pseudo : gagnants) {
		int jetons = scores.at(pseudo).nbJetonsNature;
		if (jetons > maxJetons) {
			maxJetons = jetons;
			gagnantsJetons = { pseudo };
		}
		else if (jetons == maxJetons) {
			gagnantsJetons.push_back(pseudo);
		}
	}

	return gagnantsJetons;
}


// WARNING: NE MARCHE PAS: probleme au niveau du placement des tuiles
void testScore() {
	EnvJoueur j("mario");
	std::cout << "joueur cree\n";

	// tuile 1 : regles du jeu page 9 : 23 tuiles
	// LIGNE 1
	std::array<Habitat, 6> h1 = { Habitat::prairie, Habitat::fleuve, Habitat::fleuve,
								   Habitat::fleuve, Habitat::prairie, Habitat::prairie };
	std::vector<Faune> f1 = { Faune::renard, Faune::ours };
	Tuile t1(h1, f1);
	std::cout << "av placement \n";
	j.placerTuile(Position(0, -4, +4), t1);
	std::cout << "placer tuile \n";
	j.confirmerPlacement();
	std::cout << "placement confirme \n";


	std::cout << "TUILE 1 PLACEE\n";


	std::array<Habitat, 6> h2 = { Habitat::prairie, Habitat::prairie, Habitat::prairie,
							   Habitat::foret, Habitat::foret, Habitat::foret}; 
	std::vector<Faune> f2 = { Faune::renard, Faune::ours };
	Tuile t2(h2, f2);
	Position po2(1, -4, 3);
	j.placerTuileDefinitive(po2, t2);
	JetonFaune jf2(Faune::ours);
	j.placerJetonFaune(po2, jf2);

	//LIGNE 2
	std::array<Habitat, 6> h3 = { Habitat::fleuve, Habitat::prairie, Habitat::prairie,
							   Habitat::prairie, Habitat::fleuve, Habitat::fleuve};
	std::vector<Faune> f3 = { Faune::buse};
	Tuile t3(h3, f3);
	Position po3(-1,-3,-4);
	j.placerTuileDefinitive(po3, t3);
	JetonFaune jf3(Faune::buse);
	j.placerJetonFaune(po3, jf3);

	std::array<Habitat, 6> h4 = { Habitat::fleuve, Habitat::fleuve, Habitat::prairie,
						   Habitat::prairie, Habitat::prairie, Habitat::fleuve };
	std::vector<Faune> f4 = { Faune::ours};
	Tuile t4(h4,f4);
	Position po4(0,-3,3);
	j.placerTuileDefinitive(po4,t4);
	JetonFaune jf4(Faune::ours);
	j.placerJetonFaune(po4,jf4);


	std::array<Habitat, 6> h5 = { Habitat::fleuve, Habitat::fleuve, Habitat::fleuve,
						   Habitat::fleuve, Habitat::fleuve, Habitat::fleuve };
	Faune faune5 = Faune::saumon;
	std::vector<Faune> f5 = { faune5 };
	Tuile t5(h5, f5);
	Position po5(1, -3, 2);
	j.placerTuileDefinitive(po5, t5);
	JetonFaune jf5(faune5);
	j.placerJetonFaune(po5, jf5);


	std::array<Habitat, 6> h6 = { Habitat::montagne, Habitat::montagne, Habitat::montagne,
						   Habitat::fleuve, Habitat::fleuve, Habitat::fleuve };
	Faune faune6 = Faune::saumon;
	std::vector<Faune> f6= { faune6 };
	Tuile t6(h6, f6);
	Position po6(2, -3, 1);
	j.placerTuileDefinitive(po6, t6);
	JetonFaune jf6(faune6);
	j.placerJetonFaune(po6, jf6);

	std::array<Habitat, 6> h7 = { Habitat::montagne, Habitat::montagne, Habitat::montagne,
					   Habitat::fleuve, Habitat::fleuve, Habitat::fleuve };
	Faune faune7 = Faune::wapiti;
	std::vector<Faune> f7 = { faune7 };
	Tuile t7(h7, f7);
	t7.pivoterHoraire();
	Position po7(3, -3, 0);
	j.placerTuileDefinitive(po7, t7);
	JetonFaune jf7(faune7);
	j.placerJetonFaune(po7, jf7);


	std::array<Habitat, 6> h8 = { Habitat::montagne, Habitat::montagne, Habitat::montagne,
				   Habitat::fleuve, Habitat::fleuve, Habitat::fleuve };
	Faune faune8 = Faune::buse;
	std::vector<Faune> f8 = { faune8 };
	Tuile t8(h8, f8);
	t8.pivoterHoraire(); 	t8.pivoterHoraire();
	Position po8(4, -3, -1);
	j.placerTuileDefinitive(po8, t8);
	JetonFaune jf8(faune8);
	j.placerJetonFaune(po8, jf8);


	// LIGNE 3
	std::array<Habitat, 6> h9 = { Habitat::prairie, Habitat::prairie, Habitat::prairie,
				   Habitat::marais, Habitat::marais, Habitat::marais};
	Faune faune9 = Faune::renard;
	std::vector<Faune> f9 = { faune9 };
	Tuile t9(h9, f9);
	Position po9(-2, -2, 4);
	j.placerTuileDefinitive(po9, t9);
	JetonFaune jf9(faune9);
	j.placerJetonFaune(po9, jf9);


	std::array<Habitat, 6> h10 = { Habitat::prairie, Habitat::prairie, Habitat::montagne,
			   Habitat::montagne, Habitat::montagne, Habitat::prairie };
	Faune faune10 = Faune::renard;
	std::vector<Faune> f10= { faune10 };
	Tuile t10(h10, f10);
	Position po10(-1, -2, 3);
	j.placerTuileDefinitive(po10, t10);
	JetonFaune jf10(faune10);
	j.placerJetonFaune(po10, jf10);

	std::array<Habitat, 6> h11 = { Habitat::prairie, Habitat::foret, Habitat::foret,
		   Habitat::foret, Habitat::prairie, Habitat::prairie };
	Faune faune11 = Faune::saumon;
	std::vector<Faune> f11 = { faune11 };
	Tuile t11(h11, f11);
	Position po11(0, -2, 2);
	j.placerTuileDefinitive(po11, t11);
	JetonFaune jf11(faune11);
	j.placerJetonFaune(po11, jf11);

	std::array<Habitat, 6> h12 = { Habitat::montagne, Habitat::montagne, Habitat::marais,
	   Habitat::marais, Habitat::marais, Habitat::montagne};
	Faune faune12 = Faune::buse;
	std::vector<Faune> f12= { faune12 };
	Tuile t12(h12, f12);
	Position po12(1, -2, 1);
	j.placerTuileDefinitive(po12, t12);
	JetonFaune jf12(faune12);
	j.placerJetonFaune(po12, jf12);

	std::array<Habitat, 6> h13 = { Habitat::montagne, Habitat::montagne, Habitat::marais,
	Habitat::marais, Habitat::marais, Habitat::montagne };
	Faune faune13 = Faune::wapiti;
	std::vector<Faune> f13 = { faune13 };
	Tuile t13(h13, f13);
	t13.pivoterAntiHoraire();
	Position po13(2, -2, 0);
	j.placerTuileDefinitive(po13, t13);
	JetonFaune jf13(faune13);
	j.placerJetonFaune(po13, jf13);

	std::array<Habitat, 6> h14 = {Habitat::montagne, Habitat::montagne, Habitat::marais,
			Habitat::marais, Habitat::marais, Habitat::montagne };
	Faune faune14= Faune::ours;
	std::vector<Faune> f14 = { faune14 };
	Tuile t14(h14, f14);
	Position po14(3, -2, -1);
	j.placerTuileDefinitive(po14, t14);
	JetonFaune jf14(faune14);
	j.placerJetonFaune(po14, jf14);

	//LIGNE4
	std::array<Habitat, 6> h15 = {Habitat::prairie, Habitat::prairie, Habitat::prairie,
			Habitat::prairie, Habitat::prairie, Habitat::prairie };
	Faune faune15 = Faune::wapiti;
	std::vector<Faune> f15 = { faune15};
	Tuile t15(h15, f15);
	Position po15(2, -1, 3);
	j.placerTuileDefinitive(po15, t15);
	JetonFaune jf15(faune15);
	j.placerJetonFaune(po15, jf15);


	std::array<Habitat, 6> h16 = { Habitat::foret, Habitat::foret, Habitat::foret,
		Habitat::prairie, Habitat::prairie, Habitat::prairie };
	Faune faune16 = Faune::wapiti;
	std::vector<Faune> f16= { faune16 };
	Tuile t16(h16, f16);
	Position po16(1, -1, 2);
	j.placerTuileDefinitive(po16, t16);
	JetonFaune jf16(faune16);
	j.placerJetonFaune(po16, jf16);


	std::array<Habitat, 6> h17 = { Habitat::marais, Habitat::marais, Habitat::marais,
		Habitat::foret, Habitat::foret, Habitat::foret };
	Faune faune17 = Faune::saumon;
	std::vector<Faune> f17 = { faune17 };
	Tuile t17(h17, f17);
	Position po17(0, -1, 1);
	j.placerTuileDefinitive(po17, t17);
	JetonFaune jf17(faune17);
	j.placerJetonFaune(po17, jf17);

	std::array<Habitat, 6> h18 = { Habitat::marais, Habitat::marais, Habitat::marais,
	Habitat::foret, Habitat::foret, Habitat::foret };
	Faune faune18 = Faune::wapiti;
	std::vector<Faune> f18 = { faune18 };
	Tuile t18(h18, f18);
	t18.pivoterAntiHoraire(); t18.pivoterAntiHoraire();
	Position po18(1, -1, 0);
	j.placerTuileDefinitive(po18, t18);
	JetonFaune jf18(faune18);
	j.placerJetonFaune(po18, jf18);

	std::array<Habitat, 6>h19= { Habitat::marais, Habitat::marais, Habitat::marais,
	Habitat::foret, Habitat::foret, Habitat::foret };
	Faune faune19 = Faune::ours;
	std::vector<Faune> f19 = { faune19 };
	Tuile t19(h19, f19);
	//t18.pivoterAntiHoraire(); t18.pivoterAntiHoraire();
	Position po19(2, -1, -1);
	j.placerTuileDefinitive(po19, t19);
	JetonFaune jf19(faune19);
	j.placerJetonFaune(po19, jf19);

	std::array<Habitat, 6>h20 = { Habitat::marais, Habitat::marais, Habitat::marais,
	Habitat::marais, Habitat::marais, Habitat::marais };
	Faune faune20= Faune::ours;
	std::vector<Faune> f20= { faune20 };
	Tuile t20(h20, f20);
	Position po20(3, -1, -2);
	j.placerTuileDefinitive(po20, t20);
	//JetonFaune jf20(faune20);
	//j.placerJetonFaune(po20, jf20);

	// LIGNE5
	std::array<Habitat, 6>h21 = { Habitat::foret, Habitat::foret, Habitat::foret,
				Habitat::prairie, Habitat::prairie, Habitat::prairie};
	Faune faune21 = Faune::ours;
	std::vector<Faune> f21 = { faune21 };
	Tuile t21(h21, f21);
	Position po21(1, 0, 1);
	j.placerTuileDefinitive(po21, t21);
	JetonFaune jf21(faune21);
	//j.placerJetonFaune(po21, jf21);


	std::array<Habitat, 6>h22 = { Habitat::foret, Habitat::foret, Habitat::foret,
			Habitat::marais, Habitat::marais, Habitat::marais };
	Faune faune22 = Faune::buse;
	std::vector<Faune> f22 = { faune22 };
	Tuile t22(h22, f22);
	Position po22(0, 0, 0);
	j.placerTuileDefinitive(po22, t22);
	JetonFaune jf22(faune22);
	j.placerJetonFaune(po22, jf22);


	std::array<Habitat, 6>h23 = { Habitat::foret, Habitat::foret, Habitat::foret,
			Habitat::foret, Habitat::foret, Habitat::foret };
	Faune faune23 = Faune::renard;
	std::vector<Faune> f23 = { faune23 };
	Tuile t23(h23, f23);
	Position po23(1, 0, -1);
	j.placerTuileDefinitive(po23, t23);
	JetonFaune jf23(faune23);
	j.placerJetonFaune(po23, jf23);



	j.setNbJetonsNature(2);

	Partie p;
	p.ajouterJoueur(j);
	p.setMarquage(Marquage::A);
	p.setVariante(Variante::standard);

	//Score::ScoreFeuille scoreFeuille{};
	//scoreFeuille.calculScoresPartie(p);
	//afficheScoreFeuille(scoreFeuille);
	//afficheGagnants(scoreFeuille);

}
