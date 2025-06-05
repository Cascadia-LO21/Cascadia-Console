/// Veuillez vous assurer que la version de votre IDE est au moins C++17 avant de compiler.
/// Pour assurer le bon fonctionnement des inclusions des fichiers headers, modifiez les paramètres en suivant
/// Projet > Propriétés du projet > C/C++ > Général > Répertoires
/// Puis, ajoutez : $(projet/include)include; $(projet/console)console

#include <iostream>
#include "enums.h"
#include "position.h"
#include "tuile.h"
#include "jeton_faune.h"
#include "pioche.h"
#include "env_joueur.h"
#include "partie.h"
#include "carte_marquage.h"
#include "gestion_pieces.h"
#include "score.h"
#include "saisie.h"
#include "affichage.h"

int main() {
	//std::cout << "hello world\n" << std::endl;

	// 0. Variables utilitaires
	char tmp;
	unsigned int rep = 0;
	bool jetonNatureUsed = false;

	unsigned int indexTuile = 0, indexJeton = 0;
	Position posTuile, posJeton; 
	bool placementReussi = false;
	Tuile tuileTmp; 

	// 1. Creation de la partie
	Partie p;
	std::cout << "[JEU CASCADIA]\n";

	// 2. Saisie des joueurs
	std::cout << "> Saisie des joueurs : " << std::endl;

	do {
		std::string nom;
		std::cout << "\n>> Saisis le nom d'un joueur a ajouter : ";
		std::cin >> nom;
		p.ajouterJoueur(nom);
		std::cout << "\n>> Ajouter encore ? (o/n) : ";
		std::cin >> tmp;
		if (tmp == 'n') break;
	} while (p.getNbJoueurs() < p.getMaxNbJoueurs());

	// 3. Preparer le partie avant de demarrer (verifs + distribuer tuiles de depart)
	p.preparer();

	std::cout << "> Tout est pret : joueurs, pioche, tuiles de depart distribuees.\n";
	std::cout << "> La partie peut commencer !\n" << std::endl;

	// 4. Boucle principale du jeu
	while (!p.verifierFinPartie()) { // tant qu'il reste des tours à jouer avant d'atteindre NB_MAX_TOURS
		// 4.1 Jouer
		std::cout << "> TOUR " << (p.getCompteurTour() + 1) << " / " << p.getMaxNbTours() << std::endl;
	
		std::cout << ">> Au tour de '" << p.getEnvJoueurCourant().getPseudo() <<
			"' de jouer (tour " << p.getCompteurTour() + 1 << ")" << std::endl;

		// 4.1.1 Afficher le plateau du joueur ET la pioche
		EnvJoueur& player = p.getEnvJoueurCourantModifiable();
		std::cout << player << std::endl;

		// 4.1.2 Detecter si 4 Jetons Identiques dans Pioche : si oui, on les enleve, sans les remettre dans le sac de Jetons
		while (p.getPioche().quatreJetonsIdentiques()) {
			affichePioche(p);
			std::cout << "\n>> Oh non ! Tous les jetons sont identiques dans la pioche ! Reglons cela !\n";
			p.getPiocheModifiable().resetAllJetonFaune();
		}

		// 4.1.3 Detecter si 3 jetons identiques : option de reset (une seule fois)
		char tmp;
		if (p.getPioche().troisJetonsIdentiques()) {
			affichePioche(p);
			std::cout << "\n>> 3 Jetons identiques dans la pioche ! Les retire-t-on ? (o/n) : ";
			std::cin >> tmp;
			if (tmp == 'o') p.getPiocheModifiable().resetTroisJetonsIdentiques();
		}

		std::cout << "\n>> Voici la pioche stable. Tu peux choisir mainteanant : \n";
		affichePioche(p);


		// 4.1.4 Demander si usage Jeton Nature
		if (player.getNbJetonsNature() > 0) {
			std::cout << "\n>> Veux-tu utiliser un Jeton Nature ?\n";
			std::cout << "\n>> Tu peux :\
                      \n\t1. Choisir de ne pas utiliser. \
                      \n\t2. Choisir une paire (Tuile,Jeton) libre. \
                      \n\t3. Remplacer les Jetons que tu n'aimes pas.\n";
			std::cout << ">> Saisis ton choix (1-3) : ";
			rep = saisirNombre(3);

			if (rep != 1) {
				jetonNatureUsed = true;
				player.decNbJetonsNature();
			}
		}


		// 4.1.4.1 Si Jeton Nature recouru pour remplacer les Jetons indesirables
		if (rep == 3) { 
			std::vector<unsigned int> jetonsIndesirables{};
			do {
				std::cout << "\n>> Quel jeton enlever ? ";
				jetonsIndesirables.push_back(saisirNombre(p.getPioche().getMax()));
				std::cout << "\n>> Encore ? (o/n) : ";
				std::cin >> tmp;
				if (tmp == 'n') break;
			} while (tmp == 'o' && jetonsIndesirables.size() < p.getPioche().getMax());
			p.getPiocheModifiable().resetJetonFaune(jetonsIndesirables);
		}


		// 4.1.5 Boucle principale pour gerer le choix de tuile et de jeton, et la possibilité de recommencer
		while (!placementReussi) {
			// A. Choix d'une Tuile
			affichePioche(p);
			std::cout << "\n>> Quelle tuile veux-tu '" << player.getPseudo() << "' ? \n";
			indexTuile = saisirNombre(p.getPioche().getMax());
			p.getPiocheModifiable().retirerTuileVisible(indexTuile);
			affichePioche(p); // la pioche presente une Tuile absente

			posTuile = saisirPositionTuile(p); 
			tuileTmp = p.getPioche().getPiocheVisible().at(indexTuile).first; //creer une copie
			//player.placerTuile(posTuile, tuileTmp);

			// B. Demander s'il regrette son placement de Tuile
			std::cout << "\n>>> Veux-tu revenir en arriere pour choisir une autre tuile ? (o/n)";
			std::cin >> tmp;
			if (tmp == 'o') {
				p.getPiocheModifiable().setVisibilite(indexTuile, true); // la tuile redevient visible
				player.undoDernierPlacement(); // attention: retour de la fonction non recupere
				affichePioche(p); // la pioche affiche l'etat precedent avec toutes les pieces
				continue; // On recommence le choix de la tuile
			}

			// C. Choix d'un Jeton Faune
			if (jetonNatureUsed && rep == 2) { // le joueur peut choisir librement le Jeton qu'il veut
				std::cout << "\n>> Quel jeton faune veux-tu ?\n ";
				indexJeton = saisirNombre(p.getPioche().getMax());
			}
			else { // sans jeton nature, le jeton est automatiquement celui associé a la tuile
				indexJeton = indexTuile; // meme indice
			}
			p.getPiocheModifiable().retirerJetonVisible({ indexJeton });
			affichePioche(p); // la pioche doit presenter une Tuile absente et un Jeton absent

			// D. Tentative de placement du jeton
			Faune faune = p.getPioche().getPiocheVisible().at(indexJeton).second.getType();
			posJeton = saisirPositionJeton(p,faune);
			int succesJetonPlace = player.placerJetonFaune(posJeton, faune);

			// E. Gestion des cas d'erreur de placement du jeton
			if (succesJetonPlace == -1) {
				std::cout << "\n>>> Aucune tuile ne peut accueillir ce jeton. Veux-tu recommencer ? (o/n) : ";
				std::cin >> tmp;
				if (tmp == 'o') {
					// remettre la tuile et le jeton a la pioche visible
					p.revenir(indexTuile, indexJeton);
					affichePioche(p); //la pioche est complete
					continue; // On recommence le choix de la tuile
				}
			}
			else if (succesJetonPlace == 0) {
				do {
					std::cout << "\n>>> Impossible de placer le Jeton sur la Tuile choisie !";
					std::cout << "\n>>> Veux-tu :\n\t1. Saisir une nouvelle position\n\t2. Recommencer depuis le choix de la tuile\n";
					std::cout << ">>> Saisis ton choix (1 ou 2) : ";
					unsigned int choix = saisirNombre(2);
					if (choix == 2) {
						p.revenir(indexTuile, indexJeton);
						affichePioche(p); //la pioche est complete
						break; // on sort de la boucle do...while, puis continue dans la boucle principale
					}

					// si choix = 1
					posJeton = saisirPositionJeton(p, faune);
					succesJetonPlace = player.placerJetonFaune(posJeton, faune);
				} while (succesJetonPlace != 1);

				if (succesJetonPlace == 1) {
					placementReussi = true;
				}
				// Si le joueur a choisi de recommencer, on continue dans la boucle principale
				// (donc pas besoin de faire placementReussi = true ici)
				continue;
			}

			else if (succesJetonPlace == 1) {
				placementReussi = true;
				if (tuileTmp.getDonneJetonNature()) player.incNbJetonsNature();
			}
		}


		// 4.1.6 Jeton Placé, alors la position de la Tuile est definitive
		p.apresPlacementDefinitif(posTuile, indexTuile);
		std::cout << "\n>> Les pieces sont placees dans ton plateau avec succes !";
		//if (p.getNbJoueurs() == 1) std::cout << "L'ordinateur a enleve une paire aussi. \n";
		std::cout << player;



		p.prochainJoueur(); // si jeu solitaire, alors le prochain joueur sera toujours lui-meme
		p.incCompteurTour();


		// TODO: review: 4.2 Systeme de pause/reprise de la partie (a mieux implementer dans la version graphique du jeu)
		//std::cout << "> Souhaitez-vous faire une pause ? (o/n)" << std::endl;
		//std::cin >> tmp;
		//if (tmp == 'o') p.pauser();
		//while (p.getPause()) {
		//	std::cout << ">> Reposez-vous..." << std::endl;
		//	std::cout << ">> Souhaitez-vous reprendre la partie ? (o/n)" << std::endl;
		//	std::cin >> tmp;
		//	if (tmp == 'o') p.reprendre();
		//}

	}


	// 5. Calcul des scores



	std::cout << "[FIN DU JEU CASCADIA]" << std::endl;

	return 0;
}