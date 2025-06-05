#pragma once
#include <ostream>
#include "enums.h"
#include "position.h"
#include "tuile.h"
#include <unordered_map>
#include <unordered_set>
#include "env_joueur.h"
#include "partie.h"
#include <algorithm>


namespace EvalScore {
	// declaration des methodes pour le score habitat
	class ScoreHabitat {
		// l'ensemble de tous les joueurs < chaque joueur < taille max de chaque habitat
		std::vector<std::array<int, 5>> tailleMaxTous;
		// l'ensemble de tous les joueurs < chaque joueur < son classement pour chaque habitat
		std::vector<std::array<int, 5>> bonusTous;
	public:
		void caclulScoreHabitat(Partie& partie);
		void updateTailleMaxTous(int indice, std::array<std::vector<std::vector<Position>>, 5> carteIndexee);
		void updateBonusTous(int nbJoueurs);
		void updateScore(int indice, EnvJoueur& joueur);
		void parcourHabitat(Position position, Habitat currentHabitat, const std::unordered_map<Position, Tuile>& carte, 
			std::vector<Position> positionDirectionVisitees, std::unordered_map<Position, 
			std::array<bool, 6>> carteAParcourir, std::vector<Position> zone);

		// prendre la liste des joueurs et attribut respectivement leur score habitat
		std::array<std::vector<std::vector<Position>>, 5> repartitionZone(const EnvJoueur& joueur);
		std::unordered_map<Position, std::array<bool, 6>> parcourirCarte(const std::unordered_map<Position, Tuile>& carte);
		std::vector<int> indiceMemeHabitat(Habitat habitat, const std::array<Habitat, 6> ensemble, std::array<bool, 6> dejaVisite);

		int retournerMax(std::array<std::vector<std::vector<Position>>, 5> carteIndexee, int indice);

		int getFirstFalseIndex(const std::array<bool, 6>& directions);
		void setTailleMaxTous(int taille) {
			if (taille < 1 || taille > 4) {
				throw std::out_of_range("La taille doit  tre entre 1 et 4.");
			}
			tailleMaxTous.resize(taille);
		}
		void setBonusTous(int taille) {
			if (taille < 1 || taille > 4) {
				throw std::out_of_range("La taille doit  tre entre 1 et 4.");
			}
			bonusTous.resize(taille, std::array<int, 5>{});
		}
		std::vector<std::array<int, 5>> getTailleMaxTous() { return tailleMaxTous; }
		std::vector<std::array<int, 5>> getBonusTous() { return bonusTous; }
	};

	void calculScoreFaune(std::array<int, 5> cartesMarquageChoisies, Partie& partie);



	/// Classe abstraite avec methode virtuelle pure methode calcul
	class CarteMarquage {
		Faune faune;
	public:
		CarteMarquage(Faune f) : faune(f) {}
		virtual int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const = 0;
		Faune getFaune() const { return faune; }
		virtual ~CarteMarquage() = default; //a redefinir dans classes filles
	};

	class CarteSaumon : public CarteMarquage {
	public:
		CarteSaumon() : CarteMarquage(Faune::saumon) {}
		int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const override;
		int explorerChaineSaumonA(const std::unordered_map<Position, Tuile>& carte, const Position& position, std::unordered_set<Position>& positionsVisitees, const Position* pere) const; //fct recursive qui sera appel e dans methodeCalculA, retourn la taille d'une chaine
	};

	class CarteOurs : public CarteMarquage {
	public:
		CarteOurs() : CarteMarquage(Faune::ours) {}
		int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const override;
	};

	class CarteBuse : public CarteMarquage {
	public:
		CarteBuse() : CarteMarquage(Faune::buse) {}
		int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const override;

	};

	class CarteRenard : public CarteMarquage {
	public:
		CarteRenard() : CarteMarquage(Faune::renard) {}
		int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const override;
	};

	class CarteWapiti : public CarteMarquage {
	public:
		CarteWapiti() : CarteMarquage(Faune::wapiti) {}
		int methodeCalculA(const std::unordered_map<Position, Tuile>& carte) const override;
		int explorerChaineWapitiA(const std::unordered_map<Position, Tuile>& carte, const Position& position, std::unordered_set<Position>& PositionsVisitees, std::optional<Direction> direction) const;
	};
}