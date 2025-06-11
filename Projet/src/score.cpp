#include "score.h"
#include "carte_marquage.h"

// appliquer toutes les cartes marquage X sur le player, et ces resultats seront stockes dans un ScoreJoueur
void Score::calculFauneLettre(const EnvJoueur& player, ScoreJoueur& sj, char lettre) {
	std::vector<Faune> faunes = { Faune::buse, Faune::ours, Faune::renard, Faune::saumon, Faune::wapiti };
	for (auto f : faunes) {
		std::string nomCarte = fauneToString(f) + lettre; // exemple : "saumonA"
		auto carte = CarteMarquageFactory::creerCarte(nomCarte);
		USI points = carte->CalculScore(player);
		sj.pointsFaunes[f] = points;
	}
}

// TODO
//void Score::CalculScoreFauneFamiliale::calculPointsFaunes(const EnvJoueur& player, ScoreJoueur& sj) const {
//	auto carteFamiliale = CarteMarquageFactory::creerCarte("familiale");
//	std::vector<Faune> faunes = { Faune::buse, Faune::ours, Faune::renard, Faune::saumon, Faune::wapiti };
//	for (auto f : faunes) {
//
//	}
//}
