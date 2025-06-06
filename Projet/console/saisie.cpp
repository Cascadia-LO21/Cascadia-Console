#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <limits>
#include "saisie.h"
#include "affichage.h"

// Verifie si le nombre saisi par le user est correct en terme de bornes et de caractere numerique
unsigned int saisirNombre(unsigned int max) {
    unsigned int tmp;

    while (true) {
        std::cout << "\n>>> Entre un nombre (entre 1 et " << max << ") : ";
        if (std::cin >> tmp) {
            //tmp--; // car du cote users, les indices commencent a 1

            if (tmp >= 1 || tmp <= max) {
                break; // saisie valide
            }

        }

        if (!std::cin >> tmp || tmp<1 || tmp >max) {
            std::cout << "\n>>>> Erreur : veuillez saisir un nombre entier." << std::endl;
            std::cin.clear(); // réinitialise l’état de cin
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // vide le buffer
        }
    }

    //std::cout << "Le choix choisi est : tmp = " << tmp << "\n";
    return tmp;
}

bool saisirReponse() {
    std::string rep;
    std::vector<std::string> positif = { "o", "oui", "yes", "y", "ok","1"};
    std::vector<std::string> negatif = { "n", "non", "no", "0" };
    while (true) {
        std::cin >> rep;
        std::transform(rep.begin(), rep.end(), rep.begin(), ::tolower); // convertir en minuscule
        
        if (std::find(positif.begin(), positif.end(), rep) != positif.end()) return true;
        if (std::find(negatif.begin(), negatif.end(), rep) != negatif.end())  return false;
        else std::cout << ">> Saisie invalide. Faut repondre par o/n : ";
    }
}


// Pour placer une tuile ou un jeton, on doit demander au joueur la position où placer
const Position saisirPositionTuile(const Partie& p) {
    EnvJoueur player = p.getEnvJoueurCourant();
    bool posValide = false;
    Tuile tuile;
    Direction dir;
    std::string tmp;
    //int q, r, s;
    Position pos;

    std::cout << player; // affiche le plateau du joueur

    // 1. On affiche toutes les positions des tuiles qui ont des places libres valides autour
    // 2. On lui demande de choisir la Tuile (la Position plus precisement) a cote de laquelle placer sa nouvelle Tuile
    std::cout << "\n>> Voici toutes les Tuiles a cote desquelles tu peux placer ta nouvelle tuile :" << std::endl;
    while (!posValide) {

        std::vector<Tuile> tuilesLib = player.getTuilesAvecVoisinLibre();
        std::cout << "JAI LES TUILES QUI SONT ENCORE LIBRE AUTOUR\n";

        std::cout << "JE VEUX LES AFFICHER\n";
        std::cout << tuilesLib ;

        //std::cout << "\n>> Rentre les 3 coordonnées de la Tuile à côté de laquelle tu veux placer.\n"
        //    << ">> Repète les étapes suivantes 3 fois : saisis un nombre puis appuie sur ENTRER.\n";
        //std::cout << "\n>> Coordonnée 1 : "; std::cin >> q;
        //std::cout << ">> Coordonnée 2 : "; std::cin >> r;
        //std::cout << ">> Coordonnée 3 : "; std::cin >> s;

        pos = saisirCoordonnees();

        if (!player.aTuile(pos)) {
            std::cout << "\n>> Attention: tu n'as pas de tuile placee a cette position! Veille a bien choisir parmi celles proposees !" << std::endl;
            continue; // on redemande une nouvelle position
        }
        else {
            tuile = *player.getTuile(pos);
            posValide = true;
        }
    }

    // 3. On lui affiche les directions disponibles autour de la Tuile qu'il a choisit
    std::cout << "\n>> Voici les directions libres à partir de la tuile que tu viens de choisir :\n";
    std::cout << ">> CHOISIR PARMI : " << player.getDirLibresAutourTuile(tuile);

    // 4. On lui demande la direction qu'il veut
    std::cout << "\n>> Saisis la direction que tu veux parmi celles valides : NE, E, SE, SO, O, NO :\n";
    std::cin >> tmp;
    dir = stringToDirection(tmp); //WARNING: verif a faire, ca fonctionne bien si le user est normal

    // 5. Retourner la position finale choisie
    return tuile.getPosition().getPositionAdjacente(dir);

}

// retourne une position où il y a une tuile qui peut accueillir la faune f
const std::optional<Position> saisirPositionJeton(const Partie& p, Faune f) {
    const EnvJoueur& player = p.getEnvJoueurCourant();
    bool posValide = false;
    Position pos;
    //int q, r, s;

    std::cout << player;

    // 1. Lister toutes les positions des tuiles qui peuvent accueillir la faune en question : tuile libre + vecteur de Faunes contient la faune
    // 2. Saisir les coordonnes de la position
    while (!posValide) {
        const std::vector<Position> posLibres = player.getPosLibresPourJeton(f);
  
        // si aucune Tuile ne peut accueillir la faune
        if (posLibres.size() == 0) { return std::nullopt; } 

        // sinon, si certaines tuiles peuvent accueillir la faune
        std::cout << "\nVoici les positions qui peuvent accueillir la faune " << fauneToString(f) << "\n";
        for (const auto& p : posLibres) {
            std::cout << ">> " << p << "\n";
        }

        // demande a l'utilisateur quelle position il veut
        //std::cout << "\n>> Rentre les 3 coordonnées de la Tuile sur laquelle tu veux placer.\n"
                  //<< "Repète les étapes suivantes 3 fois : saisis un nombre puis appuie sur ENTRER.\n";
        //std::cout << "\n>> Coordonnée 1 : "; std::cin >> q;
        //std::cout << ">> Coordonnée 2 : "; std::cin >> r;
        //std::cout << ">> Coordonnée 3 : "; std::cin >> s;

        pos = saisirCoordonnees();
        if (!player.aTuile(pos)) { // si aucune Tuile n'existe a cette Position
            std::cout << "\n>> Attention: tu n'as pas de tuile placee a cette position! Veille a bien choisir parmi celles proposees !" << std::endl;
            continue; // on redemande une nouvelle position
        }
        else {
            posValide = true;
        }
    }

    return pos;

}

// retourne une Position dont les 3 coordonnees sont saisies par l'utilisateur
Position saisirCoordonnees() {
    int q, r, s;

    while (true) {
        std::cout << "\n> Entre la coordonnee 1 : ";
        if (!(std::cin >> q)) { // si la donnee saisie n'est pas un int
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // vider le buffer
            std::cout << "Entree invalide, saisis un entier STP.\n";
            continue;
        }
        break;
    }

    while (true) {
        std::cout << "> Entre la coordonnee 2 : ";
        if (!(std::cin >> r)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entree invalide, saisis un entier STP.\n";
            continue;
        }
        break;
    }

    while (true) {
        std::cout << "> Entre la coordonnee 3 : ";
        if (!(std::cin >> s)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entree invalide, saisis un entier STP.\n";
            continue;
        }
        break;
    }
    return Position(q, r, s);
}


void saisirJoueurs(Partie& p) {
    std::cout << "> Saisie des joueurs : " << std::endl;
    //char tmp;
    do {
        std::string nom;
        std::cout << "\n>> Saisis le nom d'un joueur a ajouter : ";
        std::cin >> nom;
        p.ajouterJoueur(nom);
        std::cout << "\n>> Ajouter encore ? (o/n) : ";
        //std::cin >> tmp;
        if (!saisirReponse()) break;
    } while (p.getNbJoueurs() < p.getMaxNbJoueurs());
}
