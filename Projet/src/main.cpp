/// Veuillez vous assurer que la version de votre IDE est au moins C++17 avant de compiler.
/// Pour assurer le bon fonctionnement des inclusions des fichiers headers, modifiez les paramètres en suivant
/// Projet > Propriétés du projet > C/C++ > Général > Répertoires
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

int main() {
	std::cout << "hello world\n" << std::endl;
		
	//JetonFaune j(Faune::saumon);
	//std::cout << j;

	try {
		testHexagonalDisplay();
		std::cout << "Tests completed successfully!\n";
	}
	catch (const std::exception& e) {
		std::cerr << "Error during testing: " << e.what() << std::endl;
		return 1;
	}
	
}