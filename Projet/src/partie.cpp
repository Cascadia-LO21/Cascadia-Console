#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <stack>
#include <algorithm>
#include "partie.h"
#include "gestion_pieces.h"

// Avant le démarrage d'une partie, la pioche doit être initialiser.
void Partie::initialiserPioche(int nbJoueur) {
	pioche = std::make_unique<Pioche>(nbJoueur);
}

// Ajoute un nouveau EnvJoueur aux vecteur de EnvJoueur
void Partie::ajouterJoueur(const EnvJoueur& joueur) {
    if (joueurs.size() >= nbJoueurs) {
        throw std::runtime_error("Impossible d'ajouter un joueur : nombre maximum atteint (" + std::to_string(nbJoueurs) + ")");
    }
    joueurs.push_back(joueur); // copie du joueur
    nbJoueurs++;
}

// Cette surcharge permet d'écrire par exemple: ajouterJoueur("toto")
void Partie::ajouterJoueur(const std::string& nom) {
    if (joueurs.size() >= nbJoueurs) {
        throw std::runtime_error("Impossible d'ajouter un joueur : nombre maximum atteint (" + std::to_string(nbJoueurs) + ")");
    }
    joueurs.emplace_back(nom); // creation et ajout d'un EnvJoueur
    nbJoueurs++;
}

// Affiche les pseudos de tous les joeurs.
void Partie::afficheJoueurs() const {
    if (joueurs.empty())
        throw std::runtime_error("Aucun joueur encore.");
    for (const auto& j : joueurs)
        std::cout << j << std::endl;
}

void Partie::affichePioche() const {
    if (pioche) std::cout << *pioche;
    else std::cout << "La pioche est à créer.";
}

// TODO: in progress
// Gere le tour individuel d'un joueur, notamment si utiliser Jeton Nature ou pas
void Partie::jouerTourIndividuel() {
    // 0.1 Afficher le plateau du joueur ET la pioche
    EnvJoueur player = joueurs.at(joueurCourant);
    std::cout << player << std::endl;
    affichePioche();

    // 0.2 Detecter si Jetons Identiques dans Pioche : si oui, on les enleve, sans les remettre dans le sac de Jetons
    while (pioche->quatreJetonsIdentiques()) {
        pioche->resetAllJetonFaune();
        std::cout << "\n>>> Oh non ! Tous les jetons sont identiques ! Reglons cela !\n" << pioche;
    }

    // 0.3 Trois jetons identiques : option de reset (une seule fois)
    char tmp;
    if (pioche->troisJetonsIdentiques()) {
        std::cout << "\n>>> 3 Jetons identiques dans la pioche ! Les retire-t-on ? (o/n) : ";
        std::cin >> tmp;
        if (tmp == 'o') pioche->resetTroisJetonsIdentiques();
    }

    affichePioche();

    // 1. Demander si usage Jeton Nature
    unsigned int rep = 0;
    bool jetonNatureUsed = false;
    if (player.getNbJetonsNature() > 0) {
        std::cout << "\n>>> Veux-tu utiliser un Jeton Nature ?\n";
        std::cout << "\n>>> Tu peux :\
                      \n\t1. Choisir de ne pas utiliser. \
                      \n\t2. Choisir une paire (Tuile,Jeton) libre. \
                      \n\t3. Remplacer les Jetons que tu n'aimes pas.\n";
        std::cout << ">>> Saisis ton choix (1-3) : ";
        rep = saisirNombre(3); 

        if (rep != 1) jetonNatureUsed = true;

        // 1.bis. Si Jeton Nature recouru pour remplacer les Jetons indesirables
        if (rep == 3) {
            std::vector<unsigned int> jetonsIndesirables;
            while (jetonsIndesirables.size() < pioche->getMax()) {
                std::cout << "\n>>> Quel jeton enlever ? ";
                jetonsIndesirables.push_back(saisirNombre(pioche->getMax()));
                std::cout << "\n>>> Encore ? (o/n) : ";
                std::cin >> tmp;
                if (tmp == 'n') break;
            }
            pioche->resetJetonFaune(jetonsIndesirables);
            affichePioche();
        }
    }

    // Variable pour stocker le choix de tuile et de jeton
    unsigned int tuile = 0, jeton = 0;
    Position posTuile, posJeton;
    bool placementReussi = false;

    // 2. Boucle principale pour gérer le choix de tuile et de jeton, et la possibilité de recommencer
    while (!placementReussi) {
        // 2.1 Choix d'une Tuile
        std::cout << "\n>>> Quelle tuile veux-tu ?\n ";
        tuile = saisirNombre(pioche->getMax());
        pioche->retirerTuileVisible(tuile);
        affichePioche(); // la pioche presente une Tuile absente

        posTuile = saisirPositionTuile();
        player.placerTuile(posTuile, pioche->getPiocheVisible().at(tuile).first);

        // 2.1.1 Demander s'il regrette son placement de Tuile
        std::cout << "\n>>> Veux-tu revenir en arriere pour choisir une autre tuile ? (o/n)";
        std::cin >> tmp;
        if (tmp == 'o') {
            pioche->setVisibilite(tuile, true); // la tuile redevient visible
            player.undoDernierPlacement(); // attention: retour de la fonction non recupere
            affichePioche(); // la pioche affiche l'etat precedent avec toutes les pieces
            continue; // On recommence le choix de la tuile
        }

        // 2.2 Choix d'un Jeton Faune
        if (jetonNatureUsed && rep == 2) { // le joueur peut choisir librement le Jeton qu'il veut
            std::cout << "\n>>> Quel jeton faune veux-tu ?\n ";
            jeton = saisirNombre(pioche->getMax());
        }
        else { // sans jeton nature, le jeton est automatiquement celui associé a la tuile
            jeton = tuile; // meme indice
        }
        pioche->retirerJetonVisible({ jeton });
        affichePioche(); // la pioche doit presenter une Tuile absente et un Jeton absent

        // Tentative de placement du jeton
        posJeton = saisirPositionJeton();
        int succesJetonPlace = player.placerJetonFaune(posJeton, pioche->getPiocheVisible().at(jeton).second);

        // 2.2.1 Gestion des cas d'erreur de placement
        if (succesJetonPlace == -1) {
            std::cout << "\n>>> Aucune tuile ne peut accueillir ce jeton. Veux-tu recommencer ? (o/n)";
            std::cin >> tmp;
            if (tmp == 'o') {
                pioche->setVisibilite(tuile, true);
                player.undoDernierPlacement();
                affichePioche();
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
                    pioche->setVisibilite(tuile, true);
                    player.undoDernierPlacement();
                    affichePioche();
                    break; // on sort de la boucle do...while, puis continue dans la boucle principale
                }

                // si choix = 1
                posJeton = saisirPositionJeton();
                succesJetonPlace = player.placerJetonFaune(posJeton, pioche->getPiocheVisible().at(jeton).second);
            } while (succesJetonPlace == 0);

            if (succesJetonPlace == 1) {
                placementReussi = true;
            }
            // Si le joueur a choisi de recommencer, on continue dans la boucle principale
            // (donc pas besoin de faire placementReussi = true ici)
            continue;
        }

        else if (succesJetonPlace == 1) {
            placementReussi = true;
        }
    }

    // 3. Jeton Place, alors la position de la Tuile est definitive
    player.confirmerPlacement();
    std::cout << "\n>>> Les pieces sont placees dans ton plateau avec succes !";
    std::cout << player;
    pioche->slide(0, true); // slide tuiles
    pioche->slide(0, false); // slide jetons

    // 4. Si joueur solitaire, alors son tour est toujours suivi de l'extraction artificielle d'une paire de la pioche
    if (nbJoueurs == 1) {
        pioche->retirerTuileJetonDebut();
        std::cout << pioche;
        pioche->slide(0, true); // slide tuiles
        pioche->slide(0, false); // slide jetons
    }

    // 5. remplissage de la pioche visible en piochant des pieces de la pioche cachee
    pioche->rafraichirPioche();
}


// Gere les tours de chaque joueur dans le tour courant. Fonctionne bien aussi avec 1 joueur
void Partie::jouerTourCollectif() {
    std::cout << ">> " << joueurs.at(joueurCourant).getPseudo() << 
        " joue (tour " << compteurTour +1 << ")" << std::endl;

    jouerTourIndividuel();
    prochainJoueur(); // si jeu solitaire, alors le prochain joueur sera toujours lui-meme

}

// Controle le deroulement d'une partie entiere.
// Suppose d'avoir defini les joueurs et instancier la pioche pour pouvoir commencer à jouer sereinement.
void Partie::lancer() {
    saisirJoueurs();

    /// 1. Verifications des conditions preliminaires pour que le jeu puisse etre demarre.
    if (joueurs.empty()) {
        throw std::runtime_error("Impossible de démarrer : aucun joueur encore.");
    }

    // S'assurer que les attributs sont dans des états primitifs
    if (nbJoueurs != joueurs.size()) nbJoueurs = static_cast<int>(joueurs.size());
    if (joueurCourant != 0) joueurCourant = 0;
    if (compteurTour != 0) compteurTour = 0;

    // Vérifier si la pioche est initialisée: si non, on l'initialise alors
    if (!pioche) { initialiserPioche(nbJoueurs); }

    // distribuer une tuile de depart à chaque joueur : celle-ci proviennent de la Pioche, instancié à partir de donnees JSON
    if (nbJoueurs > pioche->getTuilesDepartDispo().size()) // pas assez de tuiles de depart pour le nombre total de joueurs
        throw std::runtime_error(std::to_string(pioche->getTuilesDepartDispo().size()) +
            "tuile de depart pour " + std::to_string(nbJoueurs) + "joueurs : il en manque ! \n" +
            "Veuillez ajouter de nouvelles tuiles de depart ou bien diminuer le nombre de joueurs.");

    for (unsigned int i = 0; i < nbJoueurs; i++) {
        joueurs.at(i).setTuilesDepart(GestionPieces::piocherTuileDepart(pioche->getTuilesDepartDispo()));
    }

    std::cout << "\n[JEU CASCADIA]\n";
    std::cout << "> Tout est pret : joueurs, pioche, tuiles de depart distribuees.\n";
    std::cout << "> La partie peut commencer !\n" << std::endl;

    /// 2. Boucle principale du jeu
    while (!verifierFinPartie()) { // tant qu'il reste des tours à jouer avant d'atteindre NB_MAX_TOURS
        std::cout << "> TOUR " << (compteurTour + 1) << " / " << MAX_NB_TOURS << std::endl;
        jouerTourCollectif();
        compteurTour++;

        // Systeme de pause/reprise de la partie (a mieux implementer dans la version graphique du jeu)
        char tmp;
        std::cout << ">> Souhaitez-vous faire une pause ? (o/n)" << std::endl;
        std::cin >> tmp;
        if (tmp == 'o') pauser();
        while (pause) {
            std::cout << ">>> Reposez-vous..." << std::endl;
            std::cout << ">> Souhaitez-vous reprendre la partie ? (o/n)" << std::endl;
            std::cin >> tmp;
            if (tmp == 'o') reprendre();
        }     

    }

    // TODO
    /// 3. Afficher le résultat final
    //calculerScores();
    //afficherScores();
    //calculerGagnant();
    //afficherGagnant();

    std::cout << "[FIN DU JEU CASCADIA]" << std::endl;
}

void Partie::reset() {
    joueurs.clear();
    scores = std::nullopt;
    pioche = nullptr;
    nbJoueurs = 0;
    compteurTour = 0;
    gagnant = std::nullopt;
    reprendre(); // ou: pause = false;
    marquage = Marquage::A;
    variante = Variante::standard;
    std::cout << "La partie a été réinitialisée." << std::endl;
}

// exemple : avec 20 tours, dès que le compteur fini son 20e tour, le jeu s'arrete
bool Partie::verifierFinPartie() const {
    return compteurTour == MAX_NB_TOURS + 1;
}

// TODO: verifier que les methodes sont bien implementees dans EnvJoueur.
//void Partie::calculerScores() {
//    if (scores.has_value()) throw std::runtime_error("Des scores existent déjà.");
//
//    scores = std::vector<int>{};
//    for (auto& j : joueurs) {
//        j.calculScore();
//        scores->push_back(j.getScore());
//    }
//}

// Affiche le score de chaque joueur.
void Partie::afficherScores() const {
    if (!scores.has_value()) 
        std::cout << "Aucun score à afficher !" << std::endl;
    else {
        std::cout << "[SCORES]" << std::endl;
        for (unsigned int i = 0; i < nbJoueurs; i++) {
            std::cout << joueurs.at(i).getPseudo() << " : " << scores->at(i) << std::endl;
        }
    }
}

// Cette fonction permet de stocker les indices du ou des gagnants.
// Elle suppose que les scores de chaque joueur sont deja stockés dans scores,
// grâce à calculerScores()
void Partie::calculerGagnant(){
    if (joueurs.empty()) throw std::runtime_error("Aucun joueur existant.");

    if (scores && compteurTour == MAX_NB_TOURS) {
        gagnant = std::nullopt; // s'assurer que aucun gagnant existe encore
        gagnant = std::vector<int>{}; // initialiser l'attribut optionnel
        int maxScore = *std::max_element(scores->begin(), scores->end());
        for (unsigned int i = 0; i < nbJoueurs; i++) {
            if (scores->at(i) == maxScore)
                gagnant->push_back(i);
        }
    } else {
        std::cout << "La partie n'est pas terminée encore." << std::endl;
    }
}

// Affiche le ou les gagnants (en cas d'ex aequo).
// Suppose d'avoir calculerGagnant() au prealable.
void Partie::afficherGagnant() const {
    if (!gagnant)
        throw std::runtime_error("\nVeuillez calculer les gagnants d'abord, avant d'afficher les gagnants.");
    
    gagnant->size() == 1 ? std::cout << "* GAGNANT * \n" : std::cout << "* GAGNANTS * \n";
    for (auto i : *gagnant) {
        std::cout << joueurs.at(i).getPseudo() << " avec " << scores->at(i) << " pts !" << std::endl;
    }
}


void Partie::saisirJoueurs() {
    std::string nom;
    char tmp;
    while (nbJoueurs < MAX_NB_JOUEURS) {
        std::cout << "\n>>> Saisis le nom d'un joueur a ajouter : ";
        std::cin >> nom;
        ajouterJoueur(nom);
        std::cout << "\n>>> Encore? (o/n) : ";
        std::cin >> tmp;
        if (tmp == 'n') break;
    }
}


// Verifie si le nombre saisi par le user est correct en terme de bornes et de caractere numerique
unsigned int Partie::saisirNombre(unsigned int max) const {
    int tmp;

    while (true) {
        std::cout << ">>> Entre un nombre (entre 1 et " << max << ") : ";
        if (std::cin >> tmp) {
            tmp--; // car du cote users, les indices commencent a 1

            if (tmp >= 0 || tmp < max) {
                break; // saisie valide
            }
           
        }
        else {
            std::cout << ">>>> Erreur : veuillez saisir un nombre entier." << std::endl;
            std::cin.clear(); // réinitialise l’état de cin
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // vide le buffer
        }
    }

    return tmp;
}

// TODO
// Pour placer une tuile ou un jeton, on doit demander au joueur la position où placer.
// Pour cela, on va lui demander 
// 1. A coté de quelle Tuile existante placer
// 2. Afficher les Positions disponibles autour de celle-ci
// 3. Demander quelle direction (NordOuest, etc)
// 4. Retourner la position finale choisie
//const Position& Partie::saisirPositionTuile() const {
//    EnvJoueur player = joueurs.at(joueurCourant);
//    std::cout << player; // affiche le plateau du joueur
//    int q, r, s;
//
//    //TODO : utiliser des methodes de EnvJoueur
//
//}
//
//// TODO
//const Position& Partie::saisirPositionJeton() const {
//}