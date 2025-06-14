/// Veuillez vous assurer que la version de votre IDE est au moins C++17 avant de compiler.
/// Pour assurer le bon fonctionnement des inclusions des fichiers headers, modifiez les paramètres en suivant
/// Projet > Propriétés du projet > C/C++ > Général > Répertoires
/// Puis, ajoutez : $(projet/include)include; $(projet/console)console

#include <iostream>
#include "saisie.h"
#include "affichage.h"
#include "gestion_partie.h"

int main() {

	// 1. Creation de la partie
	Partie p;
	afficherMessageBienvenu();
	choisirVariante(p); 
	choisirMarquage(p); 

	// 2. Saisie des joueurs
	saisirJoueurs(p);

	// 3. Preparer le partie avant de demarrer (verifs + distribuer tuiles de depart)
	p.preparer();
	afficherMessagePret(p);

	// 4. Boucle principale du jeu
	while (!p.verifierFinPartie()) { // tant qu'il reste des tours à jouer avant d'atteindre NB_MAX_TOURS
		afficherTour(p);
		gererJetonsIdentiques(p);
		jouerTour(p);
		gestionPause(p);
		if (abandonner(p)) return 0;
	}
	afficheEnvJoueurCourant(p); // pour mieux comparer les scores a la fin	

	// 5. Calcul des scores
	Score::ScoreFeuille scoreFeuille{};
	scoreFeuille.calculScoresPartie(p);
	afficheScoreFeuille(scoreFeuille);
	afficheGagnants(scoreFeuille);

	afficherMessageFin();

	return 0;
}