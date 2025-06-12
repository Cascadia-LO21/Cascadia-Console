#pragma once
#include <ostream>
#include <exception>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include "enums.h"
#include "env_joueur.h"
#include "partie.h"

using USI = unsigned int;

namespace Score {

	// Stockage des resultats : informations de score pour un EnvJoueur (correspond a "une colonne" dans la feuille de score)
	struct ScoreJoueur {
		std::unordered_map<Faune, USI> pointsFaunes; // exemple: "ours" correspond a 11 points gagnes
		std::unordered_map<Habitat, USI> pointsHabitats; // exemple : "marais" correspond a 7 points gagnes
		std::unordered_map<Habitat, USI> pointsHabitatsBonus; // exemple : "marais" correspond a 2 points de bonus
		USI nbJetonsNature = 0; 
		USI totalFaunes = 0, totalHabitats = 0, totalFinal = 0;
	};

	void calculFauneLettre(const EnvJoueur& player, ScoreJoueur& sj, std::string lettre);
	void calculFauneVariante(const EnvJoueur& player, ScoreJoueur& sj, std::string variante);


	// Design Pattern Strategy pour permettre de choisir le mode de calul des Marquages de Faunes selon les variantes
	class CalculScoreFaune {
	public:
		virtual ~CalculScoreFaune() = default;
		virtual void calculPointsFaunes(const EnvJoueur& player, ScoreJoueur& sj) const = 0;
	};

	// Calcul des points selon les cartes de marquage A, B, C, D...
	class CalculScoreFauneStandard : public CalculScoreFaune {
		std::string lettre;
	public:
		CalculScoreFauneStandard(std::string l) : lettre(l) {}
		void calculPointsFaunes(const EnvJoueur& player, ScoreJoueur& sj) const override {
			calculFauneLettre(player, sj, lettre);
		}
	};

	class CalculScoreFauneVariante : public CalculScoreFaune {
		std::string variante;
	public:
		CalculScoreFauneVariante(std::string v) : variante(v) {}
		void calculPointsFaunes(const EnvJoueur& player, ScoreJoueur& sj) const override {
			calculFauneVariante(player, sj, variante);
		}
	};


	// Simule la feuille de score relle : confronte les scores des EnvJoueur entre eux, puis calcul des bonus
	class ScoreFeuille {
		std::unordered_map<std::string, ScoreJoueur> scores; // exemple: le joueur "toto" correspond a une structure ScoreJoueur
		std::unique_ptr<CalculScoreFaune> strategieFaune;

		void calculTotalFaunes(ScoreJoueur& sj) {
			sj.totalFaunes = 0;
			for (const auto& [faune, points] : sj.pointsFaunes)
				sj.totalFaunes += points;
		}
		void calculTotalHabitats(ScoreJoueur& sj) {
			sj.totalHabitats = 0;
			for (const auto& [habitat, points] : sj.pointsHabitats)
				sj.totalHabitats += points;
		}
		void calculTotalFinal(ScoreJoueur& sj) {
			sj.totalFinal = sj.totalFaunes + sj.totalHabitats + sj.nbJetonsNature;
			for (const auto& [habitat, bonus] : sj.pointsHabitatsBonus)
				sj.totalFinal += bonus;
		}

	public:
		ScoreFeuille() :scores{}, strategieFaune(nullptr) {} // la feuille sera completee au fur et a mesure

		void setStrategieFaune(std::unique_ptr<CalculScoreFaune> strat) {
			strategieFaune = std::move(strat);
		}

		const ScoreJoueur& getScore(const std::string& nom) const { return scores.at(nom); }
		const std::unordered_map<std::string, ScoreJoueur>& getScores() const { return scores; }


		// utiliser patron Strategy pour calculer les points de Faune selon les variantes
		//void calculPointsFaunes(const EnvJoueur& player, ScoreJoueur& sj, Marquage m = Marquage::A);

		void calculPointsHabitats(const EnvJoueur& player, ScoreJoueur& sj);
		void calculerBonusHabitats(const std::vector<EnvJoueur>& players);


		void calculScoresPartie(const Partie&);

		std::vector<EnvJoueur> getGagnant() const;
	};
}
