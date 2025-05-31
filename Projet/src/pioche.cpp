#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <stdexcept>
#include <utility>
#include "pioche.h"
#include "enums.h"
#include "gestion_pieces.h"
using namespace GestionPieces;


void Pioche::setTuilesDispo(unsigned int nbJoueurs) {
	std::vector<Tuile> tuiles_reperes = instancierTuiles("json/tuiles_reperes.json");
	std::vector<Tuile> tuiles_non_reperes = instancierTuiles("json/tuiles_non_reperes.json");
	std::vector<Tuile> ensemble_tuiles = fusionnerVecteursTuiles(tuiles_reperes, tuiles_non_reperes); 
	GestionPieces::melangerTuiles(ensemble_tuiles); 
	adapterTailleVecteurTuiles(ensemble_tuiles, nbJoueurs);
	tuilesDispo = vectorToStack(ensemble_tuiles);
}

void Pioche::setJetonsDispo() {
	jetonsDispo = instancierJetonsFaunes();
	melangerJetons(jetonsDispo); 
}

void Pioche::setTuilesDepartDispo() {
	tuilesDepartDispo = instancierTuilesDepart();
	melangerTuilesDepart(tuilesDepartDispo);
}

// permet de changer la visibilite à l'index i : 
// si le param tuile est vrai, alors la visibilite de la tuile est changée
// sinon c'est celle du jeton faune
void Pioche::setVisibilite(unsigned int i, bool vue, bool tuile) {
	if (i >= MAX)
		throw std::out_of_range("L'indice maximal supporté est : " + std::to_string(MAX));
	
	if (tuile) {
		if (visibilite.at(i).first != vue)
			visibilite.at(i).first = vue;
	}

	else {
		if (visibilite.at(i).second != vue)
			visibilite.at(i).second = vue;
	}
}

// Pour instancier toute la pioche visible, 
// cela suppose d'avoir les tuilesDispo et jetonDispo correctement instanciés au bon nombre
void Pioche::setPiocheVisible() {
	if (tuilesDispo.empty() || jetonsDispo.empty())
		throw std::runtime_error("Les piles de tuiles et l'ensemble des jetons faunes ne sont pas encore instancié.");
	
	for (unsigned int i = 0; i < MAX; i++) {
		setPaire(i, piocherTuile(tuilesDispo), piocherJeton(jetonsDispo));
	}
}


/// AUTRES METHODES ///

// Cas où la pioche presente 4 mêmes JetonFaune : dans ce cas, il faut automatiqument remplacer TOUS les jetons
// Rend invisible TOUS les jetons faunes, et les remplace par de nouveaux
void Pioche::resetAllJetonFaune() {
	for (int i = 0; i < MAX; ++i) retirerJetonVisible(i); // ne remet pas les jetons dans la pioche
	rafraichirPioche();
}

// Cas où la pioche presente 3 mêmes JetonFaune : dans ce cas, il est tolerer de les remplacer UNE FOIS
// Cas où coup Jeton Nature : le joeur selectionne les jetons faunes qu'ils veut remplacer
void Pioche::resetJetonFaune(const std::vector<unsigned int>& indices) {
	for (unsigned int i : indices) {
		if (i >= 0 && i < MAX) {
			if (visibilite.at(i).second) {
				retirerJetonVisible(i); // ne remet pas dans la pioche
			}
		}
		else {
			throw std::out_of_range("Indice hors intervalle pour retirer le jeton Faune");
		}
	}
	rafraichirPioche();
}

void Pioche::resetTroisJetonsIdentiques() {
	std::vector<unsigned int> indicesJetonsIndentiques;
	Faune fauneIdentique = Faune::rien;

	// recherche de la Faune qui se repete
	std::array<unsigned int, 5> fauneCount{ 0 }; // initialise les compteurs à 0
	for (const auto& paire : piocheVisible) {
		unsigned int indiceFaune = static_cast<unsigned int>(paire.second.getType());
		fauneCount[indiceFaune]++;
	}

	// la Faune recherchee doit etre apparue 3 fois
	for (int i : fauneCount) {
		if (i == 3) fauneIdentique = static_cast<Faune>(i);
	}

	// recherche des indices de ces 3 Faunes identiques
	if (fauneIdentique != Faune::rien) {
		for (unsigned int i = 0; i < MAX; i++) {
			if (piocheVisible.at(i).second.getType() == fauneIdentique) 
				indicesJetonsIndentiques.push_back(i);
		}
	}

	// remplacer ces 3 faunes identiques
	resetJetonFaune(indicesJetonsIndentiques);
}

// retire une paire fixe : coup "normal"
// ne pas remettre la Tuile et le JetonFaune dans la pioche, car le joueur en sera en possession
void Pioche::retirerPaire(unsigned int i) {
	retirerTuileVisible(i);
	retirerJetonVisible(i); 
}

// retire une paire libre : coup "Jeton Nature"
// ne pas remettre la Tuile et le JetonFaune dans la pioche, car le joueur en sera en possession
void Pioche::retirerPaire(unsigned int i, unsigned int j) {
	retirerTuileVisible(i);
	retirerJetonVisible(j);
}

// rend invisible une Tuile 
void Pioche::retirerTuileVisible(unsigned int indexTuile)
{
	checkIndex(indexTuile);
	visibilite.at(indexTuile).first = false;
}

// rend invisible un JetonFaune, et le remet dans la pioche cachée si remettre = true
void Pioche::retirerJetonVisible(unsigned int i, bool remettre)
{
	checkIndex(i);
	if (visibilite.at(i).second) visibilite.at(i).second = false;

	if(remettre)
		remettreJeton(jetonsDispo, piocheVisible.at(i).second);
}

// test s'il y a n jetons faunes identiques dans la pioche visible
bool Pioche::jetonsIdentiques(int n) const {
	std::array<int, 5> fauneCount{ 0 }; // initialise les compteurs à 0
	for (const auto& paire: piocheVisible) {
		int indiceFaune = static_cast<int>(paire.second.getType());
		fauneCount[indiceFaune]++;
	}

	for (int value : fauneCount) {
		if (value == n) {
			return true;
		}
	}

	return false;
}

bool Pioche::quatreJetonsIdentiques() const { return jetonsIdentiques(4); }

bool Pioche::troisJetonsIdentiques() const { return jetonsIdentiques(3); }


// Param i : la place vide dans la ligne des Tuiles ou des Jetons
// Param tuile : indique si on traite les tuiles ou les jetons de la pioche visible
// But : Decale les elements visibles apres i jusqu'à i, et donc la fin regroupe les elements non visibles
// Exemple : slide(1, true) : decalage fait au niveau des Tuiles de la piocheVisible
// WARNING : ne conserve pas l'ordre des elements non visibles, mais seulement des elements visibles
// REMARQUE : dans une optique d'extension, cette fonction marche pour toute taille du tableau considere
void Pioche::slide(unsigned int i, bool tuile) {
	checkIndex(i);
	if (tuile) {
		unsigned int j = i;
		while (j < MAX && !visibilite[j].first) ++j; // recherche du premier element visible apres i
		for (; j < MAX; ++j) {  // on décale tous les éléments visibles vers i
			if (visibilite[j].first) {
				std::swap(piocheVisible[i].first, piocheVisible[j].first);
				std::swap(visibilite[i].first, visibilite[j].first);
				++i;
			}
		}
	}
	else {
		unsigned int j = i;
		while (j < MAX && !visibilite[j].second) ++j;
		for (; j < MAX; ++j) {
			if (visibilite[j].second) {
				std::swap(piocheVisible[i].second, piocheVisible[j].second);
				std::swap(visibilite[i].second, visibilite[j].second);
				++i;
			}
		}
	}
}


// simule un "robot" qui retire une paire en cas de joueur solitaire
// cela s'applique aussi bien, quand un joueur chosit une paire fixe ou libre (après jeton nature)
// Mais de toute maniere, on utilisera cette fonction pour retirer la premiere paire, apres avoir "Slider" toute la piocheVisbile vers le debut, 
// de sorte à avoir les places vides à la fin du tableau
void Pioche::retirerTuileJetonDebut() {
	// Retire la première tuile visible
	for (int i = 0; i < MAX; ++i) {
		if (visibilite.at(i).first) {
			visibilite.at(i).first = false;
			break;
		}
	}
	// Retire le premier jeton visible
	for (int j = 0; j < MAX; ++j) {
		if (visibilite.at(j).second) {
			visibilite.at(j).second = false;
			break;
		}
	}
}


// permet de boucher les "trous" dans la piocheVisible
void Pioche::rafraichirPioche() {
	if (tuilesDispo.empty() || jetonsDispo.empty())
		throw std::runtime_error("La pioche cachée est vidée, impossible de completer la pioche visible pour les joueurs.");

	// boucher les "trous" au niveau des Tuiles presentees
	for (unsigned int i = 0; i < MAX; ++i) {
		auto [ancienneTuile, ancienJeton] = getPaire(i);
		if (!visibilite[i].first) {
			Tuile nouvelleTuile = piocherTuile(tuilesDispo);
			visibilite[i].first = true;
			setPaire(i, nouvelleTuile, ancienJeton);
			
		}
	}

	// boucher les "trous" au niveau des jetons presentes
	for (unsigned int i = 0; i < MAX; ++i) {
		auto [ancienneTuile, ancienJeton] = getPaire(i);
		if (!visibilite[i].second) {
			JetonFaune nouveauJeton = piocherJeton(jetonsDispo);
			visibilite[i].second = true;
			setPaire(i, ancienneTuile, nouveauJeton); 
		}
	}

}


// Affiche la pioche visible, où la visibilite de chaque elemennt est controlé par visibilite
// PIOCHE :
// T1  T2  X   T3
// J1  J2  J3  J4
//
// T1 :
// <description de la tuile 1>
// etc...
// J4:
// <description du jeton 4>
std::ostream& operator<<(std::ostream& os, const Pioche& p) {
	os << "========= PIOCHE ========= \n";
	
	const auto& visibilite = p.getVisibilite();
	size_t n = p.getPiocheVisible().size();

	// Affichage des Tuiles visibles (T1, T2, X, T3)
	for (unsigned int i = 0; i < n; ++i) {
		if (visibilite[i].first) {
			os << "T" << (i + 1) << "\t";
		}
		else {
			os << "X\t";
		}
	}
	os << "\n";

	// Affichage des Jetons visibles (J1, J2, J3, J4)
	for (unsigned int i = 0; i < n; ++i) {
		if (visibilite[i].second) {
			os << "J" << (i + 1) << "\t";
		}
		else {
			os << "X\t";
		}
	}
	os << "\n\n";
	os << "--- Infos Tuiles --- \n";

	// Détail des Tuiles Visibles
	for (unsigned int i = 0; i < n; ++i) {
		if (visibilite[i].first) { 
			os << "T" << (i + 1) << ": ";
			try { os << p.getPaire(i).first << "\n";
			}
			catch (const std::exception& e) {
				os << "Erreur : " << e.what() << "\n";
			}
		}
	}

	os << "--- Infos Jetons Faunes --- \n";
	// Détail des Jetons
	for (unsigned int i = 0; i < n; ++i) {
		if (visibilite[i].second) {
			os << "J" << (i + 1) << ": ";
			try { os << p.getPaire(i).second << "\n";
			}
			catch (const std::exception& e) {
				os << "Erreur : " << e.what() << "\n";
			}
		}
	}

	os << "\n======= FIN PIOCHE ======= \n\n" << std::endl;

	return os;
}


void testPioche(int n) {
	Pioche p = Pioche(n);
	std::cout << p;
	//std::cout << p.troisJetonsIdentiques() << std::endl;

	/// RETIRE UN COUPLE FIXE
	//p.retirerPaire(3);
	//std::cout << p;

	/// RETIRE UN COUPLE LIBRE
	p.retirerPaire(0, 3);
	std::cout << p;
	/// SLIDE 
	p.slide(0,true);
	p.slide(3,false);
	std::cout << p;

	/// RETIRE UNE seule TUILE 
	//p.retirerTuileVisible(1); //tuile 1 devient invisible
	//std::cout << p;

	//std::cout << "Nb tuiles dispo au depart : " << p.getNbTuilesDispo() << std::endl;
	//std::cout << "Nb jetons faunes dispo au depart : " << p.getNbJetonsDispo() << std::endl;

	/// JETON NATURE : RETIRE 4 JETONS, OU 4jetonsIdentiques
	//p.resetAllJetonFaune();
	//std::cout << p;

	/// JETON NATURE : RETIRE certains JETONS
	//p.resetJetonFaune({ 1,3 });
	//std::cout << p;


	/// COUP IA 
	p.retirerTuileJetonDebut();
	std::cout << p;


	/// RAFRAICHIR PIOCHE
	//p.rafraichirPioche();
	//std::cout << p;
	//std::cout << "Nb tuiles dispo au depart : " << p.getNbTuilesDispo() << std::endl;
	//std::cout << "Nb jetons faunes dispo au depart : " << p.getNbJetonsDispo() << std::endl;


	/// TEST TUILES DE DEPART
	//std::cout << "Tuiles depart dispo : " << std::endl;
	//for (const auto& t : p.getTuilesDepartDispo()) std::cout << t;

}

void testPioche1joueur() {
	Pioche p(1);
	std::cout << p;
	p.retirerPaire(2); //joueur joue 1 coup nomal
	std::cout << "le joueur a joue" << std::endl << p;
	p.retirerTuileJetonDebut(); //extraction artificielle d'une paire
	std::cout << "l'IA a joué" << std::endl << p;
	p.slide(0, true); //slide tuiles
	p.slide(0, false); //slide jetons
	std::cout << "apres slide" << std::endl << p;
	p.rafraichirPioche();
	std::cout << "rafraichissement : " << std::endl << p;
}

void testPiocheBcpJoeurs(int n) {
	// joueur 1 joue
	Pioche p(n);
	std::cout << p;
	p.retirerPaire(3); //joueur 1 joue 1 coup nomal
	std::cout << "le joueur 1 a joue" << std::endl << p;
	p.slide(0, true); //slide tuiles
	p.slide(0, false); //slide jetons
	std::cout << "apres slide" << std::endl << p;
	p.rafraichirPioche();
	std::cout << "rafraichissement : " << std::endl << p;

	// joueur 2 joue : meme flow que joueur 1, sauf si use Jeton Nature
	p.retirerPaire(0, 2); 
	std::cout << "Joueur 2 utilise JetonNature : " << std::endl << p;
	p.slide(0, true); //slide tuiles
	p.slide(0, false); //slide jetons
	std::cout << "apres slide" << std::endl << p;
	p.rafraichirPioche();
	std::cout << "rafraichissement : " << std::endl << p;
}

void testPiocheJoueurUndo(int n) {
	Pioche p(n);
	std::cout << p;
	p.retirerTuileVisible(1); // retire 1e tuile
	std::cout << "joueur a retire une Tuile" << std::endl << p;

	std::cout << "voulez-vous annuler la tuile (o/n): ";
	char r;
	std::cin >> r;
	if (r == 'o') {
		p.setVisibilite(1, true); //rendre la tuile visible
	}

	std::cout << "le joueur a remis sa Tuile" << std::endl << p;

}