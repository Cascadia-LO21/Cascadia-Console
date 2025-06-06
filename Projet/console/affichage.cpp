#include <ostream>
#include <affichage.h>

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



std::ostream& operator<<(std::ostream& flux, const Tuile& tuile) {
	const auto& habitats = tuile.getHabitats();
	const auto& faunes = tuile.getFaunes();

	flux << "TUILE : \n";

	if (tuile.getPlacementConfirme())
		flux << "\t- Position : " << tuile.getPosition() << "\n";

	flux << "\t- Habitats : ";
	for (Habitat h : habitats) {
		flux << habitatToString(h) << ", ";
	}
	flux << "\n";

	if (tuile.JetonFaunePresent()) {
		flux << "\t- Faune placée : " << fauneToString(tuile.getFaunePlace()) << ".\n";
	}
	else {
		flux << "\t- Faunes possibles : ";
		for (Faune f : faunes) {
			flux << fauneToString(f) << ", ";
		}
		flux << "\n";

		if (tuile.getDonneJetonNature())
			flux << "\t- Donne Jeton Nature.\n";
	}

	return flux;
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
	os << "\n>> PIOCHE ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

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
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

	return os;
}


std::ostream& operator<<(std::ostream& os, const EnvJoueur& env) {
	os << "\n>> ENVIRONNEMENT DE [" << env.getPseudo() << "] ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	os << "Nombre Jetons Nature: " << env.getNbJetonsNature() << "\n";

	const auto& tuiles = env.getTuiles();
	if (tuiles.empty()) {
		os << "Aucune tuile placee\n";
		//os << "\n>> FIN DE L'ENVIRONNEMENT DE '" << env.getPseudo() << "'⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯\n" << std::endl;
		std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
		return os;
	}

	// afficher le système d'affichage
	//std::cout << "\n" << std::string(25, '-');
	//os << "\nFormat d'affichage: (q,r,s): ([NON CONFIRME]) ([REPERE]) [Faune(s)] [Habitats] \n";
	os << "\nOrdre Habitats: NordEst, Est, SudEst, SudOuest, Ouest, NordOuest\n";
	//std::cout << std::string(25, '-') << "\n\n";
	//os << "Légende faune: s=saumon, o=ours, b=buse, r=renard, w=wapiti\n";

	// afficher informations tuile
	os << "Detail des tuiles dans l'environnement:\n";
	for (const auto& [pos, tuile] : tuiles) {
		os << "\n" << pos;

		if (!tuile.getPlacementConfirme()) { os << " [NON CONFIRME] "; }
		if (tuile.getDonneJetonNature()) { os << " [REPERE]"; }

		if (tuile.JetonFaunePresent()) {
			os << "\tJeton Faune Place: " << fauneToString(tuile.getFaunePlace()) << "\n";
		}
		else {
			os << "\n\tPossible de placer: ";
			for (const auto& faune : tuile.getFaunes()) {
				os << fauneToString(faune) << " ";
			}
			os << "\n";
		}

		os << "\tHabitats: ";
		for (const auto& habitat : tuile.getHabitats()) {
			os << habitatToString(habitat) << " ";
		}
		os << "\n";
	}
	//os << "\n======= FIN DE L'ENVIRONNEMENT DE " << env.getPseudo() << " =======\n";
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
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

void afficherMessageBienvenu() {
	std::cout << "[JEU CASCADIA]\n";
}

void afficherMessagePret() {
	std::cout << "\n> Tout est pret : joueurs, pioche, tuiles de depart distribuees.\n";
	std::cout << "> La partie peut commencer !\n" << std::endl;
}

void affichePioche(const Partie& p) {
	std::cout << p.getPioche();
}


void afficherTour(Partie& p) {
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

	std::cout << "> [TOUR " << (p.getCompteurTour() + 1) << " / " << p.getMaxNbTours() << "]" << std::endl;
	std::cout << ">> Au tour de [" << p.getEnvJoueurCourant().getPseudo() << "] de jouer\n";
}

void afficherMenuJetonNature() {
	std::cout << "\n>> Veux-tu utiliser un Jeton Nature ?\n";
	std::cout << "\n>> Tu peux :\
                      \n\t1. Choisir de ne pas utiliser. \
                      \n\t2. Choisir une paire (Tuile,Jeton) libre. \
                      \n\t3. Remplacer les Jetons que tu n'aimes pas.\n";
	std::cout << ">> Saisis ton choix (1-3) : ";
}

void afficherMessageFin() {
	std::cout << "[FIN DU JEU CASCADIA]\n" << std::endl;
}



