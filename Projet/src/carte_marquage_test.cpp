#include <ostream>
#include "carte_marquage.h"
#include "jeton_faune.h"

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

    Tuile t20(hab, fauBRO, false);
    Tuile t21(hab, fauSW, false);

    /*cimetiere des cout
    std::cout << "avant placement t2\n";
    std::cout << "apres placement t2\n";
    std::cout << "avant placement jeton t2\n";
    std::cout << "apres placement jeton t2\n";

    */

    Mario.placerTuileDefinitiveDepart(Position(0, 0, 0), t1);
    std::cout << "Tuile t1 placee\n";
    Mario.placerJetonFaune(Position(0, 0, 0), jetonSaumon);
    std::cout << "Jeton Wapiti place sur t1\n";

    Mario.placerTuileDefinitive(Position(0, -1, 1), t15);
    Mario.placerJetonFaune(Position(0, -1, 1), jetonOurs);
    
    Mario.placerTuileDefinitive(Position(1, -1, 0), t14); 
    Mario.placerJetonFaune(Position(1, -1, 0), jetonOurs);

    Mario.placerTuileDefinitive(Position(1, 0, -1), t4); 
    Mario.placerJetonFaune(Position(1, 0, -1), jetonWapiti);

    Mario.placerTuileDefinitive(Position(-1, 0, 1), t13);
    Mario.placerJetonFaune(Position(-1, 0, 1), jetonBuse);

    Mario.placerTuileDefinitive(Position(0, 1, -1), t16);
    Mario.placerJetonFaune(Position(0, 1, -1), jetonBuse);

    Mario.placerTuileDefinitive(Position(-1, 1, 0), t3);
    Mario.placerJetonFaune(Position(-1, 1, 0), jetonSaumon);

    Mario.placerTuileDefinitive(Position(2, -1, -1), t2); 
    Mario.placerJetonFaune(Position(2, -1, -1), jetonWapiti);

    Mario.placerTuileDefinitive(Position(3, -2, -1), t9);
    Mario.placerJetonFaune(Position(3, -2, -1), jetonWapiti);

    Mario.placerTuileDefinitive(Position(2, 0, -2), t19);
    Mario.placerJetonFaune(Position(2, 0, -2), jetonRenard);

    Mario.placerTuileDefinitive(Position(1, 1, -2), t5);
    Mario.placerJetonFaune(Position(1, 1, -2), jetonSaumon);

    Mario.placerTuileDefinitive(Position(2, 1, -3), t6);
    Mario.placerJetonFaune(Position(2, 1, -3), jetonSaumon);

    Mario.placerTuileDefinitive(Position(1, 2, -3), t17);
    Mario.placerJetonFaune(Position(1, 2, -3), jetonBuse);

    Mario.placerTuileDefinitive(Position(0, 2, -2), t7);
    Mario.placerJetonFaune(Position(0, 2, -2), jetonSaumon);

    Mario.placerTuileDefinitive(Position(-1, 2, -1), t8);
    Mario.placerJetonFaune(Position(-1, 2, -1), jetonSaumon);

    Mario.placerTuileDefinitive(Position(-2, 0, 2), t10);
    Mario.placerJetonFaune(Position(-2, 0, 2), jetonOurs);

    Mario.placerTuileDefinitive(Position(-2, 1, 1), t11);
    Mario.placerJetonFaune(Position(-2, 1, 1), jetonOurs);

    Mario.placerTuileDefinitive(Position(-2, 2, 0), t18);
    Mario.placerJetonFaune(Position(-2, 2, 0), jetonBuse);

    Mario.placerTuileDefinitive(Position(0, 3, -3), t12);
    Mario.placerJetonFaune(Position(0, 3, -3), jetonRenard);

    //Mario.placerTuileDefinitive(Position(-3, 1, 2), t20);
    //Mario.placerJetonFaune(Position(-3, 1, 2), jetonOurs);

    //Mario.placerTuileDefinitive(Position(-3, 0, 3), t21);
    //Mario.placerJetonFaune(Position(-3, 0, 3), jetonOurs);

    //Mario.placerTuileDefinitive(Position(-4, 0, 4), t20);
    //Mario.placerJetonFaune(Position(-4, 0, 4), jetonOurs);

    //Mario.placerTuileDefinitive(Position(2, -2, 0), t20);
    //Mario.placerJetonFaune(Position(2, -2, 0), jetonWapiti);

    Mario.placerTuileDefinitive(Position(3, -1, -2), t21);
    //Mario.placerJetonFaune(Position(3, -1, -2), jetonWapiti);

    Mario.placerTuileDefinitive(Position(3, 0, -3), t20);
    //Mario.placerJetonFaune(Position(3, 0, -3), jetonBuse);

    std::cout << "Avant calcul score\n";

    // cartes A

    std::cout << "\nScores des cartes A\n";

    CarteSaumonA saumonA;
    CarteBuseA buseA;
    CarteRenardA renardA;
    CarteWapitiA wapitiA;
    CarteOursA oursA;

    std::cout << "Score saumons :" << saumonA.CalculScore(Mario) << "\n";
    std::cout << "Score buses :" << buseA.CalculScore(Mario) << "\n";
    std::cout << "Score renards :" << renardA.CalculScore(Mario) << "\n";
    std::cout << "Score wapitis :" << wapitiA.CalculScore(Mario) << "\n";
    std::cout << "Score ours :" << oursA.CalculScore(Mario) << "\n";

    std::cout << "\nScores des cartes B\n";

    CarteSaumonB saumonB;
    CarteBuseB buseB;
    CarteRenardB renardB;
    CarteWapitiB wapitiB;
    CarteOursB oursB;

    std::cout << "Score saumons :" << saumonB.CalculScore(Mario) << "\n";
    std::cout << "Score buses :" << buseB.CalculScore(Mario) << "\n";
    std::cout << "Score renards :" << renardB.CalculScore(Mario) << "\n";
    std::cout << "Score wapitis :" << wapitiB.CalculScore(Mario) << "\n";
    std::cout << "Score ours :" << oursB.CalculScore(Mario) << "\n";

    std::cout << "\nScores des cartes C\n";

    CarteSaumonC saumonC;
    CarteBuseC buseC;
    CarteRenardC renardC;
    CarteWapitiC wapitiC;
    CarteOursC oursC;

    std::cout << "Score saumons :" << saumonC.CalculScore(Mario) << "\n";
    std::cout << "Score buses :" << buseC.CalculScore(Mario) << "\n";
    std::cout << "Score renards :" << renardC.CalculScore(Mario) << "\n";
    std::cout << "Score wapitis :" << wapitiC.CalculScore(Mario) << "\n";
    std::cout << "Score ours :" << oursC.CalculScore(Mario) << "\n";

    std::cout << "\nVariante famille \n";
    auto varF = CarteMarquageVarianteFactory::creerCarte("famille");
    const auto& scoreFam = varF->CalculScore(Mario);
    for (const auto& [key, value] : scoreFam) {
        std::cout << key << ": " << value << std::endl;
    }

    //std::cout << "\nVariante intermediaire \n";
    //VarianteIntermediaire varI;
    //std::cout << "Score intermediaire :" << varI.CalculScore(Mario) << "\n";

    std::cout << "Fin testCartes()\n";
}