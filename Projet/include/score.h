#pragma once
#include <ostream>
#include <exception>
#include <map>
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
		std::map<Faune, USI> pointsFaunes; // exemple: "ours" correspond a 11 points gagnes
		std::map<Habitat, USI> pointsHabitats; // exemple : "marais" correspond a 7 points gagnes
		std::map<Habitat, USI> pointsHabitatsBonus; // exemple : "marais" correspond a 2 points de bonus
		USI nbJetonsNature = 0; 
		USI totalFaunes = 0, totalHabitats = 0, totalFinal = 0;
	};

	// Design Pattern Strategy pour permettre de choisir le mode de calul des Marquages de Faunes selon les variantes
	class CalculScoreFaune {
	public:
		virtual ~CalculScoreFaune() = default;
		virtual void calculPointsFaunes(const EnvJoueur& player, ScoreJoueur& sj) const = 0;
	};

	class CalculScoreFauneA : public CalculScoreFaune {
	public:
		void calculPointsFaunes(const EnvJoueur& player, ScoreJoueur& sj) const override;
	};

	class CalculScoreFauneB : public CalculScoreFaune {
	public:
		void calculPointsFaunes(const EnvJoueur& player, ScoreJoueur& sj) const override;
	};

	class CalculScoreFauneC : public CalculScoreFaune {
	public:
		void calculPointsFaunes(const EnvJoueur& player, ScoreJoueur& sj) const override;
	};

	class CalculScoreFauneD : public CalculScoreFaune {
	public:
		void calculPointsFaunes(const EnvJoueur& player, ScoreJoueur& sj) const override;
	};

	class CalculScoreFauneFamiliale : public CalculScoreFaune {
	public:
		void calculPointsFaunes(const EnvJoueur& player, ScoreJoueur& sj) const override;
	};

	class CalculScoreFauneIntermediaire : public CalculScoreFaune {
	public:
		void calculPointsFaunes(const EnvJoueur& player, ScoreJoueur& sj) const override;
	};




	// Simule la feuille de score relle : confronte les scores des EnvJoueur entre eux, puis calcul des bonus
	class ScoreFeuille {
		std::map<std::string, ScoreJoueur> scores; // exemple: le joueur "toto" correspond a une structure ScoreJoueur
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
		void setStrategieFaune(std::unique_ptr<CalculScoreFaune> strat) {
			strategieFaune = std::move(strat);
		}

		const ScoreJoueur& getScore(const std::string& nom) const { return scores.at(nom); }

		// utiliser patron Strategy pour calculer les points de Faune selon les variantes
		//void calculPointsFaunes(const EnvJoueur& player, ScoreJoueur& sj, Marquage m = Marquage::A);

		void calculPointsHabitats(const EnvJoueur& player, ScoreJoueur& sj);
		void calculerBonusHabitats(const std::vector<EnvJoueur>& joueurs);


		void calculScoresPartie(const Partie&);

	};


}

// TODO : afficher la feuille de score/classement
// TODO : afficher les scores d'un joueur