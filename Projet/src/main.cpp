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

void testPioche() {

}


int main() {
	std::cout << "hello world\n" << std::endl;
	
	//testClassePosition();

	//testClasseTuile();

	testGestionPieces();
	
	//JetonFaune j(Faune::saumon);
	//cout << j;

	// Test des cartes
}