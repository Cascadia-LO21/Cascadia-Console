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
    //std::string tmp;
    //int q, r, s;
    Position pos;

    //std::cout << player; // affiche le plateau du joueur

    // 1. On affiche toutes les positions des tuiles qui ont des places libres valides autour
    // 2. On lui demande de choisir la Tuile (la Position plus precisement) a cote de laquelle placer sa nouvelle Tuile
    std::cout << "\n>> Voici toutes les Tuiles a cote desquelles tu peux placer ta nouvelle tuile :" << std::endl;
    std::vector<Tuile> tuilesLib = player.getTuilesAvecVoisinLibre();
    std::cout << tuilesLib ;

    pos = saisirCoordonnees(player);
    tuile = *player.getTuile(pos);

    // 3. On lui affiche les directions disponibles autour de la Tuile qu'il a choisit
    std::vector<Direction> dirLibres = player.getDirLibresAutourTuile(tuile);
    dir = saisirDirection(dirLibres);

    return tuile.getPosition().getPositionAdjacente(dir);
}

// retourne une position où il y a une tuile qui peut accueillir la faune f
const std::optional<Position> saisirPositionJeton(const Partie& p, Faune f) {
    const EnvJoueur& player = p.getEnvJoueurCourant();
    bool posValide = false;
    //Position pos;
    //int q, r, s;

    std::cout << player;

    // 1. Lister toutes les positions des tuiles qui peuvent accueillir la faune en question : tuile libre + vecteur de Faunes contient la faune
    // 2. Saisir les coordonnes de la position
    const std::vector<Position> posLibres = player.getPosLibresPourJeton(f);
  
    // si aucune Tuile ne peut accueillir la faune
    if (posLibres.size() == 0) { return std::nullopt; } 

    // sinon, si certaines tuiles peuvent accueillir la faune
    std::cout << "\nVoici les positions qui peuvent accueillir la faune " << fauneToString(f) << "\n";
    for (const auto& p : posLibres) {
        std::cout << ">> " << p << "\n";
    }

    return saisirCoordonnees(player);
}

// retourne une Position dont les 3 coordonnees sont saisies par l'utilisateur
// cette Position est verifiee 
Position saisirCoordonnees(const EnvJoueur& player) {
    int q, r, s;

    do {
        while (true) {
            std::cout << "\n> Entre la coordonnee 1 : ";
            if (!(std::cin >> q)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\n> Entree invalide, saisis un entier STP.\n";
                continue;
            }
            break;
        }
        while (true) {
            std::cout << "> Entre la coordonnee 2 : ";
            if (!(std::cin >> r)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\n> Entree invalide, saisis un entier STP.\n";
                continue;
            }
            break;
        }
        while (true) {
            std::cout << "> Entre la coordonnee 3 : ";
            if (!(std::cin >> s)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\n> Entree invalide, saisis un entier STP.\n";
                continue;
            }
            break;
        }

        if (q + r + s != 0) {
            std::cout << "\n> La somme des 3 coordonnees doit faire 0, retente STP. ";
        }
        else if (!player.getTuile(Position(q, r, s))) {
            std::cout << "\n> Tu n'as pas de tuile placee ici, retente STP. ";
        }
        else {
            return Position(q, r, s);
        }
    } while (true);
}

void saisirJoueurs(Partie& p) {
    std::cout << "> Saisie des joueurs : " << std::endl;
    //unsigned int nbJoueurs = 0;
    do {
        std::string nom;
        std::cout << "\n>> Saisis le nom d'un joueur a ajouter : ";
        std::cin >> nom;
        p.ajouterJoueur(nom);
        //nbJoueurs++;
   
        if (p.getNbJoueurs() < p.getMaxNbJoueurs()) {
            std::cout << "\n>> Ajouter encore ? (o/n) : ";
            if (!saisirReponse()) break;
        }
    } while (p.getNbJoueurs() < p.getMaxNbJoueurs());
}

Direction saisirDirection(const std::vector<Direction>& dirValides) {
    std::cout << "\n>> Les cotés libres autour de la tuile sont :\n>> ";
    for (auto d : dirValides) {
        std::cout << directionToString(d) << "(" << directionToStringSigle(d) << "), ";
    }
    std::cout << std::endl;

    while (true) {
        std::cout << "\n>> Saisis le sigle d'une direction : ";
        std::string dirString; 
        std::cin >> dirString; 
        dirString = toLowerNoSpace(dirString); //convertir tout en minuscule

        Direction dir;
        try {
            dir = stringToDirection(dirString);
        }
        catch (const std::invalid_argument&) {
            std::cout << "\n>> Saisie inconnue, recommence.\n";
            continue;
        }

        // Vérifie si la direction est dans la liste des directions valides
        if (std::find(dirValides.begin(), dirValides.end(), dir) != dirValides.end()) {
            return dir;
        }
        else {
            std::cout << "\n>> Cette direction n'est pas disponible, recommence.\n";
        }
    }
}



std::string toLowerNoSpace(const std::string& s) {
    std::string res;
    for (char c : s) {
        if (c != ' ') res += std::tolower(c);
    }
    return res;
}