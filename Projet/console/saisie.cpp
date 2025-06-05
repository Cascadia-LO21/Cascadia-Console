#include <iostream>
#include <vector>
#include "saisie.h"
#include "affichage.h"

void saisirJoueur(unsigned int max, std::vector<EnvJoueur>& joueurs) {
    std::string nom;
    std::cout << "\n>>> Saisis le nom d'un joueur a ajouter : ";
    std::cin >> nom;
    joueurs.emplace_back(nom); 
}


// Verifie si le nombre saisi par le user est correct en terme de bornes et de caractere numerique
unsigned int saisirNombre(unsigned int max) {
    unsigned int tmp;

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


// Pour placer une tuile ou un jeton, on doit demander au joueur la position où placer
const Position saisirPositionTuile(Partie p) {
    EnvJoueur player = p.getEnvJoueurCourant();
    bool posValide = false;
    Tuile tuile;
    Direction dir;
    std::string tmp;
    int q, r, s;

    std::cout << player; // affiche le plateau du joueur

    // 1. On affiche toutes les positions des tuiles qui ont des places libres valides autour
    // 2. On lui demande de choisir la Tuile (la Position plus precisement) a cote de laquelle placer sa nouvelle Tuile
    std::cout << "\n>> Voici toutes les Tuiles a cote desquelles tu peux placer ta nouvelle tuile :" << std::endl;
    while (!posValide) {
        std::cout << player.getTuilesAvecVoisinLibre();
        std::cout << "\n>> Rentre les 3 coordonnées de la Tuile à côté de laquelle tu veux placer.\n"
            << ">> Repète les étapes suivantes 3 fois : saisis un nombre puis appuie sur ENTRER.\n";
        std::cout << "\n>> Coordonnée 1 : "; std::cin >> q;
        std::cout << ">> Coordonnée 2 : "; std::cin >> r;
        std::cout << ">> Coordonnée 3 : "; std::cin >> s;

        if (!player.aTuile(Position(q, r, s))) {
            std::cout << "\n>> Attention: tu n'as pas de tuile placee a cette position! Veille a bien choisir parmi celles proposees !" << std::endl;
            continue; // on redemande une nouvelle position
        }
        else {
            tuile = *player.getTuile(Position(q, r, s));
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
const Position saisirPositionJeton(Partie p, Faune f) {
    EnvJoueur player = p.getEnvJoueurCourant();
    bool posValide = false;
    int q, r, s;

    std::cout << player;

    // 1. Lister toutes les positions des tuiles qui peuvent accueillir la faune en question : tuile libre + vecteur de Faunes contient la faune
    // 2. Saisir les coordonnes de la position
    while (!posValide) {
        std::cout << "\nVoici les positions qui peuvent accueillir la faune " << fauneToString(f) << "\n";
        std::cout << player.getPosLibresPourJeton(f);
        std::cout << "\n>> Rentre les 3 coordonnées de la Tuile sur laquelle tu veux placer.\n"
            << "Repète les étapes suivantes 3 fois : saisis un nombre puis appuie sur ENTRER.\n";
        std::cout << "\n>> Coordonnée 1 : "; std::cin >> q;
        std::cout << ">> Coordonnée 2 : "; std::cin >> r;
        std::cout << ">> Coordonnée 3 : "; std::cin >> s;

        if (!player.aTuile(Position(q, r, s))) {
            std::cout << "\n>> Attention: tu n'as pas de tuile placee a cette position! Veille a bien choisir parmi celles proposees !" << std::endl;
            continue; // on redemande une nouvelle position
        }
        else {
            posValide = true;
        }
    }

    return Position(q, r, s);

}