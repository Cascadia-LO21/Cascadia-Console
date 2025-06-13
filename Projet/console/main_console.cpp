/// Veuillez vous assurer que la version de votre IDE est au moins C++17 avant de compiler.
/// Pour assurer le bon fonctionnement des inclusions des fichiers headers, modifiez les paramètres en suivant
/// Projet > Propriétés du projet > C/C++ > Général > Répertoires
/// Puis, ajoutez : $(projet/include)include; $(projet/console)console

#include <iostream>
//#include "enums.h"
//#include "position.h"
//#include "tuile.h"
//#include "jeton_faune.h"
//#include "pioche.h"
//#include "env_joueur.h"
//#include "partie.h"
//#include "carte_marquage.h"
//#include "gestion_pieces.h"
//#include "score.h"
#include "saisie.h"
#include "affichage.h"
#include "gestion_partie.h"
//#include "carte_marquage.h" // test only

int main() {

	//testCartes(); // test only, A COMMENTER

	//testScore();
	//std::cout << "FIN TEST.";

	unsigned int r = saisirNombre(5);

	// =======================================================================

	// 1. Creation de la partie
	Partie p;
	afficherMessageBienvenu();
	choisirVariante(p); // a decommenter !
	choisirMarquage(p); // a decommenter !

	// 2. Saisie des joueurs
	saisirJoueurs(p);

	// 3. Preparer le partie avant de demarrer (verifs + distribuer tuiles de depart)
	p.preparer();
	afficherMessagePret();

	// 4. Boucle principale du jeu
	while (!p.verifierFinPartie()) { // tant qu'il reste des tours à jouer avant d'atteindre NB_MAX_TOURS
		afficherTour(p);
		gererJetonsIdentiques(p);
		//afficheEnvJoueurCourant(p);
		//affichePioche(p); 
		jouerTour(p);
		gestionPause(p);
	}

	// 5. Calcul des scores
	Score::ScoreFeuille scoreFeuille{};
	scoreFeuille.calculScoresPartie(p);
	afficheScoreFeuille(scoreFeuille);
	afficheGagnants(scoreFeuille);

	afficherMessageFin();

	return 0;
}