#include <ostream>
#include <iomanip>
#include <unordered_map>
#include "env_joueur.h"
#include <affichage.h>

// affichage dans le format ligne par ligne :
//            (0,-1, 1)
//    (-1, 0, 1) (0, 0, 0) (1, 0, -1)
//        (-1, 1, 0) (0, 1, -1)
//             (-1, 2, -1)
void affichePlateau(const std::unordered_map<Position, Tuile>& plateau) {
	if (plateau.empty()) {
		std::cout << "Plateau vide.\n";
		return;
	}

	std::cout << "\n┌──────────────────────────────────────────────────────────────────────────────────────────┐\n\n";

	// determiner les bornes q/r
	int minQ = 0, maxQ = 0, minR = 0, maxR = 0;
	bool premier = true;
	for (const auto& [pos, tuile] : plateau) {
		int q = pos.getQ();
		int r = pos.getR();
		if (premier) {
			minQ = maxQ = q;
			minR = maxR = r;
			premier = false;
		}
		else {
			minQ = std::min(minQ, q);
			maxQ = std::max(maxQ, q);
			minR = std::min(minR, r);
			maxR = std::max(maxR, r);
		}
	}

	// affichage ligne par ligne, par r croissant : un r regit une ligne
	for (int r = minR; r <= maxR; ++r) {
		int decalage = (r - minR); // decalage pour l'effet hexagonal (pointy top)
		std::cout << std::string(decalage * 6, ' '); // 6 espaces par decalage

		for (int q = minQ; q <= maxQ; ++q) {
			int s = -q - r;
			Position pos(q, r, s);
			auto it = plateau.find(pos);
			if (it != plateau.end()) {
				std::cout << "(" << q << "," << std::setw(2) << r << "," << std::setw(2) << s << ") ";
			}
			else {
				std::cout << "           "; // espace pour une tuile absente
			}
		}
		std::cout << "\n";
	}
	
	std::cout << "\n└──────────────────────────────────────────────────────────────────────────────────────────┘\n";
	std::cout << "\n";
}

std::ostream& operator<<(std::ostream& os, const JetonFaune& j) {
	os << fauneToString(j.getType());
	return os;
}

std::ostream& operator<<(std::ostream& flux, const Position& p)
{
	flux << "(" << p.getQ() << "," << p.getR() << "," << p.getS() << ")";
	return flux;
}

std::ostream& operator<<(std::ostream& flux, const std::vector<Position>& vect) {
	for (const auto& p : vect)
		flux << p << " ";
	return flux;
}


// Si tuile libre dans pioche
//[REPERE]
// ┃ Faunes : buse, renard
// ┃ Habitats : fleuve(NE), riviere(E), fleuve(SE), riviere(SO), montagne(O), montagne(NO), 
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

// Si tuile libre dans Map en attente
//(0,0,0) [REPERE] [NON CONFIRME] 
// ┃ Faunes : buse, renard
// ┃ Habitats : fleuve(NE), riviere(NE), fleuve(NE), riviere(NE), montagne(NE), montagne(NE), 
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

// Si tuile confirme avec faune dans Map
//(0,0,0) [buse]
// │ Habitats : fleuve(NE), riviere(NE), fleuve(NE), riviere(NE), montagne(NE), montagne(NE), 
// └──────────────────────────────────────────────────────────────────────────────────────────┘
std::ostream& operator<<(std::ostream& os, const Tuile& tuile) {
	//os << "\n";
	if (tuile.positionDefinie()) os << tuile.getPosition() << " ";
	if (tuile.getDonneJetonNature()) os << "[REPERE] "; 
	if (tuile.positionDefinie() && !tuile.getPlacementConfirme()) { os << "[NON CONFIRME] "; }

	// faunes
	if (tuile.JetonFaunePresent()) os << "[" << fauneToString(tuile.getFaunePlace()) << "] ";
	else {
		os << "\n│ Faunes : ";
		for (auto faune : tuile.getFaunes()) {
			os << fauneToString(faune) << ", ";
		}
	}

	// habitats
	os << "\n│ Habitats: ";
	std::array<Habitat,6> hab = tuile.getHabitats();
	for (int i = 0; i < 6; i++) {
		os << habitatToString(hab[i]) << "(" << directionToStringSigle(static_cast<Direction>(i)) << "), ";
	}
	os << "\n└──────────────────────────────────────────────────────────────────────────────────────────┘\n";


	return os;
}

//ostream pour vecteur de tuile like in envjoueur
std::ostream& operator<<(std::ostream& os, const std::vector<Tuile>& tuiles) {
	if (tuiles.empty()) {
		os << "Aucune tuile à afficher\n";
		return os;
	}

	for (const auto& t : tuiles) os << t;

	return os;
}


// Affiche la pioche visible, où la visibilite de chaque elemennt est controlé par visibilite
// PIOCHE :
// T1  T2  X   T3
// J1  J2  J3  J4
//
// T1 :
// <description de la tuile 1>
// etc...
// J4:
// <description du jeton 4>
std::ostream& operator<<(std::ostream& os, const Pioche& p) {
	os << "\n>> PIOCHE ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

	const auto& visibilite = p.getVisibilite();
	size_t n = p.getPiocheVisible().size();

	// Affichage des Tuiles visibles (T1, T2, X, T3)
	os << "┌───────────────────────────┐\n";

	for (unsigned int i = 0; i < n; ++i) {
		if (visibilite[i].first) {
			os << " T" << (i + 1) << "\t";
		}
		else {
			os << " X\t";
		}
	}
	os << "\n";

	// Affichage des Jetons visibles (J1, J2, J3, J4)

	for (unsigned int i = 0; i < n; ++i) {
		if (visibilite[i].second) {
			os << " J" << (i + 1) << "\t";
		}
		else {
			os << " X\t";
		}
	}
	os << "\n└───────────────────────────┘\n\n";

	//os << "--- Infos Tuiles --- \n";

	// Détail des Tuiles Visibles
	for (unsigned int i = 0; i < n; ++i) {
		if (visibilite[i].first) {
			os << "T" << (i + 1) << ": ";
			try {
				os << p.getPaire(i).first << "\n";
			}
			catch (const std::exception& e) {
				os << "Erreur : " << e.what() << "\n";
			}
		}
	}

	//os << "--- Infos Jetons Faunes --- \n";
	// Détail des Jetons
	for (unsigned int i = 0; i < n; ++i) {
		if (visibilite[i].second) {
			os << "J" << (i + 1) << ": ";
			try {
				os << p.getPaire(i).second << "\n";
			}
			catch (const std::exception& e) {
				os << "Erreur : " << e.what() << "\n";
			}
		}
	}

	//os << "\n======= FIN PIOCHE ======= \n\n" << std::endl;
	//std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

	return os;
}


std::ostream& operator<<(std::ostream& os, const EnvJoueur& env) {
	os << "\n>> ENVIRONNEMENT DE [" << env.getPseudo() << "] ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	os << "\n> Nombre Jetons Nature : " << env.getNbJetonsNature() << "\n\n";

	const auto& tuiles = env.getTuiles();
	if (tuiles.empty()) {
		os << "Aucune tuile placee\n";
		//os << "\n>> FIN DE L'ENVIRONNEMENT DE '" << env.getPseudo() << "'⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯\n" << std::endl;
		std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
		return os;
	}
	else
		affichePlateau(tuiles);

	for (const auto& [pos, tuile] : tuiles) os << tuile;

	//std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	return os;
}


// Affiche les pseudos de tous les joeurs.
void afficheJoueurs(const Partie& p) {
	if (p.getJoueurs().empty())
		throw std::runtime_error("Aucun joueur encore.");
	for (const auto& j : p.getJoueurs())
		std::cout << j << std::endl;
}

void afficheEnvJoueurCourant(const Partie& p) {
	std::cout << p.getEnvJoueurCourant();
}

// affichage proche de vrai feuille de score dans le jeu physique
void afficheScoreFeuille(const Score::ScoreFeuille& s) {
	std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	std::cout << "> Fin de la partie ! Voici les SCORES : \n";

	std::vector<Faune> faunes = { Faune::buse, Faune::ours, Faune::renard, Faune::saumon, Faune::wapiti };
	std::vector<Habitat> habitats = { Habitat::fleuve, Habitat::foret, Habitat::marais, Habitat::montagne, Habitat::prairie };

	const auto& scores = s.getScores();

	std::vector<std::string> pseudos;
	for (const auto& [pseudo,_] : scores) {
		pseudos.push_back(pseudo);
	}

	const int col = 14; // Espacement à inserer

	// En-tete
	std::cout << std::setw(col) << " ";
	for (const auto& pseudo : pseudos) {
		std::cout << std::setw(col) << pseudo;
	}
	std::cout << "\n";

	// Faunes
	for (const auto& f : faunes) {
		std::cout << std::setw(col) << fauneToString(f);
		for (const auto& pseudo : pseudos) {
			std::cout << std::setw(col) << scores.at(pseudo).pointsFaunes.at(f);
		}
		std::cout << "\n";
	}

	// Total Faunes
	std::cout << std::setw(col) << "Total Faune";
	for (const auto& pseudo : pseudos) {
		std::cout << std::setw(col) << scores.at(pseudo).totalFaunes;
	}
	std::cout << "\n";

	// Habitats (points/bonus)
	for (const auto& h : habitats) {
		std::cout << std::setw(col) << habitatToString(h);
		for (const auto& pseudo : pseudos) {
			int pts = scores.at(pseudo).pointsHabitats.at(h);
			int bonus = scores.at(pseudo).pointsHabitatsBonus.at(h);
			std::cout << std::setw(col) << (std::to_string(pts) + "/" + std::to_string(bonus));
		}
		std::cout << "\n";
	}

	// Total Habitats
	std::cout << std::setw(col) << "Total Habitats";
	for (const auto& pseudo : pseudos) {
		std::cout << std::setw(col) << scores.at(pseudo).totalHabitats;
	}
	std::cout << "\n";

	// Jetons Nature
	std::cout << std::setw(col) << "Jetons Nature";
	for (const auto& pseudo : pseudos) {
		std::cout << std::setw(col) << scores.at(pseudo).nbJetonsNature;
	}
	std::cout << "\n";

	// Total 
	std::cout << std::setw(col) << "[Total]";
	for (const auto& pseudo : pseudos) {
		std::cout << std::setw(col) << scores.at(pseudo).totalFinal;
	}
	std::cout << "\n";
}

void afficheGagnants(const Score::ScoreFeuille& s) {
	auto gagnants = s.getGagnants();
	if (gagnants.size() == 1) {
		std::cout << "\n> Le gagnant est : " << gagnants[0] << " ! " << std::endl;
	}
	else {
		std::cout << "\n> Victoire partagee entre : ";
		for (const auto& pseudo : gagnants)
			std::cout << "\n>> " << pseudo;
		std::cout << std::endl;
	}
	std::cout << "\n";

}

void afficheMenuRotation() {
	std::cout << "\n>> Dans quel sens pivoter la tuile ?";
	std::cout << "\n\t1. Sens Horaire.";
	std::cout << "\n\t2. Sens Anti Horaire.";
}


void afficherMessageBienvenu() {
	std::cout << "[JEU CASCADIA]\n";
}

// pour simplifier, une partie est regie par le meme type de carte marquage pour toutes les faunes
// par exemple, une partie est regie par des cartes marquage A seulement
void afficherMarquage() {
	std::cout << "\n> Choisir le mode de marquage de la partie en tapant un chiffre entre 1 et 4:";
	std::cout << "\n\t1. A\n\t2. B\n\t3. C\n\t4. D\n\t5. Defaut\n\t6. Aleatoire"; 
}

void afficherVariante() {
	std::cout << "\n> Choisir la variante de la partie en tapant un chiffre entre 1 et 3:";
	std::cout << "\n\t1. Standard\n\t2. Famille\n\t3. Intermediaire";

}

void afficherMessagePret(const Partie& p) {
	std::cout << "\n╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍\n";
	std::cout << "\n> Tout est pret : joueurs, pioche, tuiles de depart distribuees.";
	std::cout << "\n> Variante : " << varianteToString(p.getVariante()) << " & " << "Marquage : " << marquageToString(p.getMarquage());
	std::cout << "\n> La partie peut commencer !\n" << std::endl;
}

void affichePioche(const Partie& p) {
	std::cout << p.getPioche();
}


void afficherTour(Partie& p) {
	//std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	//std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	std::cout << "╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍\n\n";

	std::cout << "> [TOUR " << (p.getCompteurTour() + 1) << " / " << p.getMaxNbTours() << "]" << std::endl;
	std::cout << "\n>> Au tour de [" << p.getEnvJoueurCourant().getPseudo() << "] de jouer\n";
}

void afficherMenuJetonNature() {
	std::cout << "\n>> Veux-tu utiliser un Jeton Nature ?\n";
	std::cout << "\n>> Tu peux :\
                      \n\t1. Choisir de ne pas utiliser. \
                      \n\t2. Choisir une paire (Tuile,Jeton) libre. \
                      \n\t3. Remplacer les Jetons que tu n'aimes pas.\n";
}

void afficherMessageFin() {
	std::cout << "\n[FIN DU JEU CASCADIA]\n" << std::endl;
}



