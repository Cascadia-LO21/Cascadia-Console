#include "gestion_partie.h"
#include "affichage.h"
#include "saisie.h"

void gererJetonsIdentiques(Partie& p) {
	//char tmp;
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
		if (saisirReponse()) p.getPiocheModifiable().resetTroisJetonsIdentiques();
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

    p.apresPlacementDefinitif(player);

    std::cout << "\n>> Les pieces sont placees dans ton plateau avec succes !\n";
    std::cout << player;

    // on incremente le compteur de tour qu'apres que le dernier joueur ait joué son tour
    if(p.getIndexJoueurCourant() == p.getNbJoueurs() - 1)
        p.incCompteurTour();

    p.prochainJoueur();
}

void gererJetonsIndesirables(Partie& p) {
    affichePioche(p);
    std::vector<unsigned int> jetonsIndesirables{};
    bool tmp;
    do {
        std::cout << "\n>> Quel jeton enlever ? ";
        jetonsIndesirables.push_back(saisirNombre(p.getPioche().getMax())-1); //on retire 1 car du cote user le comtpage commence a 1
        std::cout << "\n>> Encore ? (o/n) : ";
        tmp = saisirReponse();
        if (!tmp) break;
    } while (tmp && jetonsIndesirables.size() < p.getPioche().getMax());
    p.getPiocheModifiable().resetJetonFaune(jetonsIndesirables);
}

// un retour de False dans la boucle originale permet de revenir dans cette fonction, pour recommencer le choix de la tuile
// tandis qu'un retour de True explique que la Tuile et le Jeton ont ete places avec succes
bool placerTuileEtJeton(Partie& p, EnvJoueur& player, unsigned int rep, bool jetonNatureUsed) {
    unsigned int indexTuile, indexJeton;
    Position posTuile;
    std::optional<Position> posJeton;
    Tuile tuileTmp;
    //char tmp;
    int succesJetonPlace = 0;

    // Choix de la tuile
    affichePioche(p);
    afficheEnvJoueurCourant(p);
    std::cout << "\n>> Quelle tuile veux-tu [" << player.getPseudo() << "] ? \n";
    indexTuile = saisirNombre(p.getPioche().getMax())-1;
    p.getPiocheModifiable().retirerTuileVisible(indexTuile);
    affichePioche(p);

    posTuile = saisirPositionTuile(p);
    tuileTmp = p.getPioche().getPiocheVisible().at(indexTuile).first;
    player.placerTuile(posTuile, tuileTmp); //ajouter la tuile temporairement dans le plateau du joueur, faudra confirmer a la fin

    // Gestion de la rotation des tuiles
    while (true) {
        std::cout << "\n>> Veux tu pivoter la tuile ? (o/n) : ";
        if (!saisirReponse()) break;
        afficheMenuRotation(); // 1. sens horaire, 2. sens anti horaire
        unsigned int action = saisirNombre(2);
        if (action == 1)
            player.getTuileNonConst(posTuile)->pivoterHoraire();
        else if(action ==2)
            player.getTuileNonConst(posTuile)->pivoterAntiHoraire();
        afficheEnvJoueurCourant(p); // affichage apres rotation
    }

    bool donneJetonNature = tuileTmp.getDonneJetonNature();

    // Retour en arrière
    std::cout << "\n>> Veux-tu revenir en arriere pour choisir une autre tuile ? (o/n) : ";
    //std::cin >> tmp;
    if (saisirReponse()) {
        p.getPiocheModifiable().setVisibilite(indexTuile, true);
        player.undoDernierPlacement();
        //affichePioche(p);
        return false;
    }

    // Choix du jeton
    if (jetonNatureUsed && rep == 2) {
        std::cout << "\n>> Quel jeton faune veux-tu ?\n ";
        indexJeton = saisirNombre(p.getPioche().getMax())-1;
    }
    else {
        indexJeton = indexTuile;
    }
    p.getPiocheModifiable().retirerJetonVisible({ indexJeton });
    affichePioche(p);

    // Placement du jeton
    Faune faune = p.getPioche().getPiocheVisible().at(indexJeton).second.getType();
    posJeton = saisirPositionJeton(p, faune);

    if (!posJeton.has_value()) { // aucune tuile ne peut accueillir la faune
        std::cout << ">> Aucune tuile ne peut accueillir la faune : " << fauneToString(faune) << ".\n";
        std::cout << ">> Veux-tu : \n 1. Delaisser le jeton et continuer le jeu. \n 2. Recommencer le choix de tuile.\n";
        unsigned int rep = saisirNombre(2);
        if (rep == 1) return true; // continuer sans placer le jeton
        else if (rep == 2) { // refaire le choix de tuile
            p.revenir(indexTuile, indexJeton); 
            affichePioche(p);
            return false;
        }

    }
    else 
        succesJetonPlace = player.placerJetonFaune(posJeton.value(), faune);


    // Gestion des erreurs
    if (succesJetonPlace == -1) {
        //std::cout << "je suis dans la boucle si succes = -1\n";
        std::cout << "\n>>> Aucune tuile ne peut accueillir ce jeton. Veux-tu recommencer ? (o/n) : ";
        //std::cin >> tmp;
        if (saisirReponse()) {
            p.revenir(indexTuile, indexJeton);
            affichePioche(p);
            return false;
        }
    }
    else if (succesJetonPlace == 0) {
        //std::cout << "je suis dans la boucle si succes = 0\n";

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
            succesJetonPlace = player.placerJetonFaune(posJeton.value(), faune);
        } while (succesJetonPlace != 1);
    }
    if (succesJetonPlace == 1) {
        if (donneJetonNature) { 
            player.incNbJetonsNature(); 
            //std::cout << "je suis dans la boucle si succes = 1 et jai incremente nbjetonnature de joueur\n";

        }
        return true;
    }

    return false;

}


void gestionPause(Partie& p) {
    std::cout << "> Souhaitez-vous faire une pause ? (o/n) : ";
    if (saisirReponse()) p.pauser();
    while (p.getPause() == true) {
        std::cout << ">> Reposez-vous..." << std::endl;
        std::cout << ">> Souhaitez-vous reprendre la partie ? (o/n) : ";
        if (saisirReponse()) p.reprendre();
    }

}

bool abandonner(Partie& p) {
    std::cout << "> Souhaitez-vous abandonner la partie? (o/n) : ";
    if (saisirReponse()) {
        "\n> Abandon de la partie...";
        afficherMessageFin();
        return true;
    }
    return false;
}

void choisirVariante(Partie& p) {
    afficherVariante();
    unsigned int rep = saisirNombre(3);
    switch (rep)
    {
    case 1:
        p.setVariante(Variante::standard);
        break;
    case 2:
        p.setVariante(Variante::famille);
        break;
    case 3:
        p.setVariante(Variante::intermediaire);
        break;
    default:
        p.setVariante(Variante::standard);
    }
}

void choisirMarquage(Partie& p) {
    if (p.getVariante() == Variante::famille) {
        p.setMarquage(Marquage::famille);
    }
    else if (p.getVariante() == Variante::intermediaire) {
        p.setMarquage(Marquage::intermediaire);
    }
    else { // si la variante est standard, il faut demander quel type de carte marquage
        afficherMarquage();
        unsigned int rep = saisirNombre(6);
        switch (rep) {
        case 1:
            p.setMarquage(Marquage::A);
            break;
        case 2:
            p.setMarquage(Marquage::B);
            break;
        case 3:
            p.setMarquage(Marquage::C);
            break;
        case 4:
            p.setMarquage(Marquage::D);
            break;
        case 5: // defaut
            p.setMarquage(Marquage::A);
            break;
        case 6: // marquage aleatoire parmi A, B, C, D
        {
            int randomValue = 1 + rand() % 4; // génère 1, 2, 3 ou 4
            p.setMarquage(static_cast<Marquage>(randomValue - 1)); // A=0, B=1, etc.
        }
        break;
        default:
            p.setMarquage(Marquage::A);
            break;
        }
    }
}