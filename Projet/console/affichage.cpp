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
	os << "========= PIOCHE ========= \n";

	const auto& visibilite = p.getVisibilite();
	size_t n = p.getPiocheVisible().size();

	// Affichage des Tuiles visibles (T1, T2, X, T3)
	for (unsigned int i = 0; i < n; ++i) {
		if (visibilite[i].first) {
			os << "T" << (i + 1) << "\t";
		}
		else {
			os << "X\t";
		}
	}
	os << "\n";

	// Affichage des Jetons visibles (J1, J2, J3, J4)
	for (unsigned int i = 0; i < n; ++i) {
		if (visibilite[i].second) {
			os << "J" << (i + 1) << "\t";
		}
		else {
			os << "X\t";
		}
	}
	os << "\n\n";
	os << "--- Infos Tuiles --- \n";

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

	os << "--- Infos Jetons Faunes --- \n";
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

	os << "\n======= FIN PIOCHE ======= \n\n" << std::endl;

	return os;
}


std::ostream& operator<<(std::ostream& os, const EnvJoueur& env) {
	os << "======== ENVIRONNEMENT DE " << env.getPseudo() << " ========\n";
	os << "Jetons Nature: " << env.getNbJetonsNature() << " | Score Final: " << env.getScore() << "\n";

	const auto& tuiles = env.getTuiles();
	if (tuiles.empty()) {
		os << "Aucune tuile placee\n";
		os << "\n======= FIN DE L'ENVIRONNEMENT DE " << env.getPseudo() << " =======\n";
		return os;
	}

	// afficher le système d'affichage
	std::cout << "\n" << std::string(25, '-');
	//os << "\nFormat d'affichage: (q,r,s): ([NON CONFIRME]) ([REPERE]) [Faune(s)] [Habitats] \n";
	os << "\nOrdre Habitats: NordEst, Est, SudEst, SudOuest, Ouest, NordOuest\n";
	//std::cout << std::string(25, '-') << "\n\n";
	//os << "Légende faune: s=saumon, o=ours, b=buse, r=renard, w=wapiti\n";

	// afficher informations tuile
	os << "Detail des tuiles dans l'environnement:\n";
	for (const auto& [pos, tuile] : tuiles) {
		//os << "\n  (" << pos.getQ() << "," << pos.getR() << "," << pos.getS() << "): ";
		os << "\n" << pos;

		if (!tuile.getPlacementConfirme()) { os << " [NON CONFIRME] "; }
		if (tuile.getDonneJetonNature()) { os << " [REPERE]"; }

		if (tuile.JetonFaunePresent()) {
			os << "\tJeton Faune Place: " << fauneToString(tuile.getFaunePlace()) << "\n";
		}
		else {
			os << "\n\tPossibilite de placer: ";
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
	os << "\n======= FIN DE L'ENVIRONNEMENT DE " << env.getPseudo() << " =======\n";
	return os;
}
