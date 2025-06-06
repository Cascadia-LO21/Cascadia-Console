#include "gestion_partie.h"
#include "affichage.h"
#include "saisie.h"

void gererJetonsIdentiques(Partie& p) {
	char tmp;
	// 4 jetons identiques
	while (p.getPioche().quatreJetonsIdentiques()) {
		affichePioche(p);
		std::cout << "\n>> Oh non ! Tous les jetons sont identiques dans la pioche ! Reglons cela !\n";
		p.getPiocheModifiable().resetAllJetonFaune();
	}

	// 3 jetons identiques
	if (p.getPioche().troisJetonsIdentiques()) {
		affichePioche(p);
		std::cout << "\n>> 3 Jetons identiques dans la pioche ! Les retire-t-on ? (o/n) : ";
		std::cin >> tmp;
		if (tmp == 'o') p.getPiocheModifiable().resetTroisJetonsIdentiques();
	}
}


void jouerTour(Partie& p) {
    EnvJoueur& player = p.getEnvJoueurCourantModifiable();
    bool placementReussi = false;
    unsigned int rep = 0;
    bool jetonNatureUsed = false;

    // Gestion du jeton nature
    if (player.getNbJetonsNature() > 0) {
        afficherMenuJetonNature();
        rep = saisirNombre(3);
        if (rep == 2 || rep == 3) {
            jetonNatureUsed = true;
            player.decNbJetonsNature();
        }
        if (rep == 3) {
            gererJetonsIndesirables(p);
        }
    }

    // Boucle principale du placement
    while (!placementReussi) {
        placementReussi = placerTuileEtJeton(p, player, rep, jetonNatureUsed);
    }

    // Après placement définitif
    p.apresPlacementDefinitif();
    std::cout << "\n>> Les pieces sont placees dans ton plateau avec succes !";
    std::cout << player;

    p.prochainJoueur();
    p.incCompteurTour();
}

void gererJetonsIndesirables(Partie& p) {
    std::vector<unsigned int> jetonsIndesirables{};
    char tmp;
    do {
        std::cout << "\n>> Quel jeton enlever ? ";
        jetonsIndesirables.push_back(saisirNombre(p.getPioche().getMax())); 
        std::cout << "\n>> Encore ? (o/n) : ";
        std::cin >> tmp;
        if (tmp == 'n') break;
    } while (tmp == 'o' && jetonsIndesirables.size() < p.getPioche().getMax());
    p.getPiocheModifiable().resetJetonFaune(jetonsIndesirables);
}

// un retour de False dans la boucle originale permet de revenir cette fonction
// tandis qu'un retour de True explique que la Tuile et le Jeton ont ete places avec succes
bool placerTuileEtJeton(Partie& p, EnvJoueur& player, unsigned int rep, bool jetonNatureUsed) {
    unsigned int indexTuile, indexJeton;
    Position posTuile, posJeton;
    Tuile tuileTmp;
    char tmp;

    // Choix de la tuile
    affichePioche(p);
    std::cout << "\n>> Quelle tuile veux-tu '" << player.getPseudo() << "' ? \n";
    indexTuile = saisirNombre(p.getPioche().getMax());
    p.getPiocheModifiable().retirerTuileVisible(indexTuile);
    affichePioche(p);

    posTuile = saisirPositionTuile(p);
    tuileTmp = p.getPioche().getPiocheVisible().at(indexTuile).first;

    // Retour en arrière
    std::cout << "\n>>> Veux-tu revenir en arriere pour choisir une autre tuile ? (o/n)";
    std::cin >> tmp;
    if (tmp == 'o') {
        p.getPiocheModifiable().setVisibilite(indexTuile, true);
        player.undoDernierPlacement();
        affichePioche(p);
        return false;
    }

    // Choix du jeton
    if (jetonNatureUsed && rep == 2) {
        std::cout << "\n>> Quel jeton faune veux-tu ?\n ";
        indexJeton = saisirNombre(p.getPioche().getMax());
    }
    else {
        indexJeton = indexTuile;
    }
    p.getPiocheModifiable().retirerJetonVisible({ indexJeton });
    affichePioche(p);

    // Placement du jeton
    Faune faune = p.getPioche().getPiocheVisible().at(indexJeton).second.getType();
    posJeton = saisirPositionJeton(p, faune);
    int succesJetonPlace = player.placerJetonFaune(posJeton, faune);

    // Gestion des erreurs
    if (succesJetonPlace == -1) {
        std::cout << "\n>>> Aucune tuile ne peut accueillir ce jeton. Veux-tu recommencer ? (o/n) : ";
        std::cin >> tmp;
        if (tmp == 'o') {
            p.revenir(indexTuile, indexJeton);
            affichePioche(p);
            return false;
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
                affichePioche(p);
                return false;
            }
            posJeton = saisirPositionJeton(p, faune);
            succesJetonPlace = player.placerJetonFaune(posJeton, faune);
        } while (succesJetonPlace != 1);
    }
    else if (succesJetonPlace == 1) {
        if (tuileTmp.getDonneJetonNature()) player.incNbJetonsNature();
        return true;
    }
    return false;
}


void gestionPause(Partie& p) {
    std::cout << "> Souhaitez-vous faire une pause ? (o/n)" << std::endl;
    if (saisirReponse()) p.pauser();
    while (p.getPause() == true) {
        std::cout << ">> Reposez-vous..." << std::endl;
        std::cout << ">> Souhaitez-vous reprendre la partie ? (o/n)" << std::endl;
        if (saisirReponse()) p.reprendre();
    }

}