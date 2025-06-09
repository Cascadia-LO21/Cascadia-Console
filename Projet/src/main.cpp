/// Veuillez vous assurer que la version de votre IDE est au moins C++17 avant de compiler.
/// Pour assurer le bon fonctionnement des inclusions des fichiers headers, modifiez les parametres en suivant
/// Projet > Proprietes du projet > C/C++ > General > Repertoires
/// Puis, ajoutez : $(projet/include)include

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

void testCartes() {
    EnvJoueur Mario("mario");
    std::cout << "EnvJoueur Mario cree\n";

    // habitats arbitraires
    std::array<Habitat, 6> hab = { Habitat::fleuve, Habitat::fleuve, Habitat::fleuve,
                                   Habitat::foret, Habitat::foret, Habitat::foret };
    // combinaisons de faune
    std::vector<Faune> fauBRO = { Faune::buse, Faune::renard, Faune::ours };
    std::vector<Faune> fauSW = { Faune::saumon, Faune::wapiti };

    JetonFaune jetonOurs(Faune::ours);
    JetonFaune jetonSaumon(Faune::saumon);
    JetonFaune jetonRenard(Faune::renard);
    JetonFaune jetonWapiti(Faune::wapiti);
    JetonFaune jetonBuse(Faune::buse);

    // pour les tests on ne tient pas compte dess 3 tuiles de depart

    Tuile t1(hab, fauSW, false);
    std::cout << "Tuile t1 creee\n";
    Tuile t2(hab, fauSW, false);
    Tuile t3(hab, fauSW, false);
    Tuile t4(hab, fauSW, false);
    Tuile t5(hab, fauSW, false);
    Tuile t6(hab, fauSW, false);
    Tuile t7(hab, fauSW, false);
    Tuile t8(hab, fauSW, false);
    Tuile t9(hab, fauSW, false);

    Tuile t10(hab, fauBRO, false);
    Tuile t11(hab, fauBRO, false);
    Tuile t12(hab, fauBRO, false);
    Tuile t13(hab, fauBRO, false);
    Tuile t14(hab, fauBRO, false);
    Tuile t15(hab, fauBRO, false);
    Tuile t16(hab, fauBRO, false);
    Tuile t17(hab, fauBRO, false);
    Tuile t18(hab, fauBRO, false);
    Tuile t19(hab, fauBRO, false);

    Mario.placerTuileDefinitiveDepart(Position(3, -2, -1), t1);
    std::cout << "Tuile t1 placee\n";

    //Mario.placerJetonFaune(Position(3, -2, -1), jetonWapiti);

    try {
        Mario.placerJetonFaune(Position(3, -2, -1), jetonWapiti);
        std::cout << "Jeton Wapiti place sur t1\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Exception lors du placement du jeton : " << e.what() << std::endl;
    }

    //std::cout << "Jeton Wapiti place sur t1\n";

    Mario.placerTuileDefinitive(Position(2, -1, -1), t2);
    Mario.placerJetonFaune(Position(2, -1, -1), jetonWapiti);

    Mario.placerTuileDefinitive(Position(0, 0, 0), t3);
    Mario.placerJetonFaune(Position(0, 0, 0), jetonSaumon);

    Mario.placerTuileDefinitive(Position(1, 0, 1), t4);
    Mario.placerJetonFaune(Position(1, 0, 1), jetonWapiti);

    Mario.placerTuileDefinitive(Position(-1, 1, 0), t5);
    Mario.placerJetonFaune(Position(-1, 1, 0), jetonSaumon);

    Mario.placerTuileDefinitive(Position(1, 1, -2), t6);
    Mario.placerJetonFaune(Position(1, 1, -2), jetonSaumon);

    Mario.placerTuileDefinitive(Position(2, 1, -3), t7);
    Mario.placerJetonFaune(Position(2, 1, -3), jetonSaumon);

    Mario.placerTuileDefinitive(Position(-1, 2, -1), t8);
    Mario.placerJetonFaune(Position(-1, 2, -1), jetonSaumon);

    Mario.placerTuileDefinitive(Position(0, 2, -2), t9);
    Mario.placerJetonFaune(Position(0, 2, -2), jetonSaumon);

    Mario.placerTuileDefinitive(Position(0, -1, 1), t10);
    Mario.placerJetonFaune(Position(0, -1, 1), jetonOurs);

    Mario.placerTuileDefinitive(Position(1, -1, 0), t11);
    Mario.placerJetonFaune(Position(1, -1, 0), jetonOurs);

    Mario.placerTuileDefinitive(Position(-2, 0, 2), t12);
    Mario.placerJetonFaune(Position(-2, 0, 2), jetonOurs);

    Mario.placerTuileDefinitive(Position(-1, 0, 1), t13);
    Mario.placerJetonFaune(Position(-1, 0, 1), jetonBuse);

    Mario.placerTuileDefinitive(Position(2, 0, -2), t14);
    Mario.placerJetonFaune(Position(2, 0, -2), jetonRenard);

    Mario.placerTuileDefinitive(Position(-2, 1, 1), t15);
    Mario.placerJetonFaune(Position(-2, 1, 1), jetonOurs);

    Mario.placerTuileDefinitive(Position(0, 1, -1), t16);
    Mario.placerJetonFaune(Position(0, 1, -1), jetonBuse);

    Mario.placerTuileDefinitive(Position(-2, 2, 0), t17);
    Mario.placerJetonFaune(Position(-2, 2, 0), jetonBuse);

    Mario.placerTuileDefinitive(Position(1, 2, -3), t18);
    Mario.placerJetonFaune(Position(1, 2, -3), jetonBuse);

    Mario.placerTuileDefinitive(Position(0, 3, -3), t19);
    Mario.placerJetonFaune(Position(0, 3, -3), jetonRenard);

    std::cout << "Avant calcul score\n";

    // cartes A
    CarteSaumonA saumonA;
    int scoreSaumon = saumonA.CalculScore(Mario);

    std::cout << scoreSaumon;

    std::cout << "Fin testCartes()\n";
}

int main() {
    std::cout << "hello world\n" << std::endl;

    //JetonFaune j(Faune::saumon);
    //std::cout << j;
    /*
    try {
        testHexagonalDisplay();
        std::cout << "Tests completed successfully!\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error during testing: " << e.what() << std::endl;
        return 1;
    }
    */

    std::cout << "Avant testCartes()\n";
    testCartes();
    std::cout << "Apres testCartes()\n";

    return 0;
}
