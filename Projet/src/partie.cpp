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
}

// Cette surcharge permet d'écrire par exemple: ajouterJoueur("toto")
void Partie::ajouterJoueur(const std::string& nom) {
    if (joueurs.size() >= nbJoueurs) {
        throw std::runtime_error("Impossible d'ajouter un joueur : nombre maximum atteint (" + std::to_string(nbJoueurs) + ")");
    }
    joueurs.emplace_back(nom); // creation et ajout d'un EnvJoueur
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
// Gere le tour individuel pour le tour d'un joueur : gestion de Jeton Nature ou pas
void Partie::jouerTourIndividuel() {
    EnvJoueur player = joueurs.at(joueurCourant); 
    char tmp;

    /// 0.1 Afficher le plateau du joueur ET la pioche
    std::cout << player << std::endl;
    std::cout << pioche << std::endl;

    /// 0.2 Detecter si Jetons Identiques dans Pioche : si oui, on les enleve, sans les remettre dans le sac de Jetons
    while (pioche->quatreJetonsIdentiques()) {
        pioche->resetAllJetonFaune();
        std::cout << ">>> Oh non ! Tous les jetons sont identiques ! Reglons cela !\n" << pioche;
    }

    if (pioche->troisJetonsIdentiques()) {
        std::cout << ">>> 3 Jetons identiques dans la piche ! Veux-tu qu'on les retire ? (o/n) : ";
        (tmp == 'o') pioche->resetTroisJetonsIdentiques();
    }

    /// 1. Demander si usage Jeton Nature
    unsigned int rep;
    if (player.getNbJetonsNature() > 0) {
        std::cout << ">>> Veux-tu utiliser un Jeton Nature ?\n";
        std::cout << ">>> Tu peux :\
                      \n\t0. Choisir de ne pas utiliser. \
                      \n\t1. Choisir une paire (Tuile,Jeton) libre. \
                      \n\t2. Remplacer les Jetons que tu n'aimes pas.\n";
        std::cout << ">>> Saisis ton choix (0, 1 ou 2) : ";
        std::cin >> rep;
    }

    /// 1.a Si Jeton Nature -> remplacer les Jetons indesirables
    if (rep == 2) {
        std::vector<unsigned int> jetonsIndesirables;
        while (jetonsIndesirables.size() < pioche->getMax()) {
            std::cout << ">>> Quel jeton enlever ? ";
            jetonsIndesirables.push_back(saisirNombre());
            std::cout << ">>> Encore ? (o/n) : ";
            std::cin >> tmp;
            if (tmp == 'n') break;

        }
    }

    /// 1.1 Si Jeton Nature -> choix paire libre
    if (rep == 1) {
        unsigned int tuile = 0, jeton = 0;


        /// 1. choix de n'importe quelle Tuile
        while (true) {
            std::cout << ">>> Quelle tuile veux-tu ?\n ";
            tuile = saisirNombre();
            pioche->retirerTuileVisible(tuile);
            std::cout << pioche;
            player.placerTuile(saisirPositionTuile(), pioche->getPiocheVisible().at(tuile).first);

            /// 1.1 Demander s'il regrette son placement de Tuile
            std::cout << ">>>> Veux-tu revenir en arriere pour choisir autre chose? (o/n)";
            std::cin >> tmp;
            if (tmp == 'n') break;
            else {
                pioche->setVisibilite(tuile, true); // la tuile redevient visible
                player.undoDernierPlacement(); // attention: retour de la fonction non recupere
                std::cout << pioche;
            }
        }
       
        /// 2. choix de n'importe quelle Jeton Faune
        std::cout << ">>> Quel jeton faune veux-tu ?\n ";
        jeton = saisirNombre();
        pioche->retirerJetonVisible({ jeton });
        std::cout << pioche;
        int succesJetonPlace = player.placerJetonFaune(saisirPositionJeton(), pioche->getPiocheVisible().at(tuile).second);

        if(succesJetonPlace == -1)

        /// 3. Jeton Place, alors la position de la Tuile est definitive
        player.confirmerPlacement();


    }




    /// 1.3 Coup normal : pas de Jeton Nature OU apres JetonNature-remplacer-Jetons

    

    /// Si joueur solitaire, alors son tour est toujours suivi de l'extraction artififcielle d'une paire de la pioche
    if (nbJoueurs == 1) {
        jouerTourIndividuel();
        pioche->retirerTuileJetonDebut();
    }
}

// TODO
void Partie::jouerTourCollectif() {
    std::cout << ">> " << joueurs.at(joueurCourant).getPseudo() << 
        " joue (tour " << compteurTour +1 << ")" << std::endl;

    jouerTourIndividuel();
    prochainJoueur();

}

// Controle le deroulement d'une partie entiere.
// Suppose d'avoir defini les joueurs et instancier la pioche pour pouvoir commencer à jouer sereinement.
void Partie::lancer() {

    /// 1. Verifications des conditions preliminaires pour que le jeu puisse etre demarre.
    // Verifier si des joueurs existent
    if (joueurs.empty()) {
        throw std::runtime_error("Impossible de démarrer : aucun joueur encore.");
    }

    // S'assurer que les attributs sont dans des états primitifs
    if (nbJoueurs != joueurs.size()) nbJoueurs = joueurs.size();
    if (joueurCourant != 0) joueurCourant = 0;
    if (compteurTour != 0) compteurTour = 0;

    // Vérifier si la pioche est initialisée: si non, on l'initialise alors
    if (!pioche) { initialiserPioche(nbJoueurs); }

    // distribuer une tuile de depart à chaque joueur : celle-ci proviennent de la Pioche, instancié à partir de donnees JSON
    if (nbJoueurs > pioche->getTuilesDepartDispo().size()) // pas assez de tuiles de depart pour le nombre total de joueurs
        throw std::runtime_error(std::to_string(pioche->getTuilesDepartDispo().size()) +
            "tuile de depart pour " + std::to_string(nbJoueurs) + "joueurs : il en manque ! \n" +
            "Veuillez ajouter de nouvelles tuiles de depart ou bien diminuer le nombre de joueurs.");

    for (int i = 0; i < nbJoueurs; i++) {
        //joueurs.at(i).setTuilesDepart(pioche->getTuilesDepartDispo().at(i));
        std::vector<std::vector<Tuile>> tuilesDepart = pioche->getTuilesDepartDispo();
        joueurs.at(i).setTuilesDepart(GestionPieces::piocherTuileDepart(tuilesDepart));
    }

    std::cout << "[JEU CASCADIA]\n";
    std::cout << "La pioche est prete et les tuiles de depart ont ete distribuees a chacun des joueurs.\n";
    std::cout << "La partie peut commencer !\n" << std::endl;

    /// 2. Boucle principale du jeu
    while (!verifierFinPartie()) { // tant qu'il reste des tours à jouer avant d'atteindre NB_MAX_TOURS
        std::cout << "> TOUR " << (compteurTour + 1) << " / " << MAX_NB_TOURS << std::endl;
        jouerTourCollectif();
        compteurTour++;

        // Systeme de pause/reprise de la partie (a mieux implementer dans la version graphique du jeu)
        char p = 'o', r = 'n';
        std::cout << ">> Souhaitez-vous faire une pause ? (o/n)" << std::endl;
        std::cin >> p;
        while (p == 'o' && r == 'n') {
            pause();
            std::cout << ">>> Reposez-vous..." << std::endl;
            std::cout << ">> Souhaitez-vous reprendre la partie ? (o/n)" << std::endl;
            std::cin >> r;
        }
            

    }

    /// 3. Afficher le résultat final
    calculerScores();
    afficherScores();
    calculerGagnant();
    afficherGagnant();
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
        for (int i = 0; i < nbJoueurs; i++) {
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
        for (int i = 0; i < nbJoueurs; i++) {
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
        throw std::runtime_error("Veuillez calculer les gagnants d'abord, avant d'afficher les gagnants.");
    
    gagnant->size() == 1 ? std::cout << "* GAGNANT * \n" : std::cout << "* GAGNANTS * \n";
    for (auto i : *gagnant) {
        std::cout << joueurs.at(i).getPseudo() << " avec " << scores->at(i) << " pts !" << std::endl;
    }
}

// Verifie si le nombre saisi par le user est correct en terme de bornes et de caractere numerique
unsigned int Partie::saisirNombre() const {
    int tmp;

    while (true) {
        std::cout << ">>> Entre un nombre (entre 0 et " << pioche->getMax() << ") : ";
        if (std::cin >> tmp) {
            tmp--; // car du cote users, les indices commencent a 1

            if (tmp >= 0 || tmp < n) {
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
const Position& Partie::saisirPositionTuile() const {
    EnvJoueur player = joueurs.at(joueurCourant);
    std::cout << player; // affiche le plateau du joueur
    int q, r, s;

    //TODO : utiliser des methodes de EnvJoueur

}

// TODO
const Position& Partie::saisirPositionJeton() const {
}