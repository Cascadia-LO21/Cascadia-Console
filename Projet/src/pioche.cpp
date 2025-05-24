#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <stdexcept>
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
	for (int i = 0; i < MAX; ++i) retirerJetonVisible(i, true); // true : remet les jetons dans la pioche
	rafraichirPioche();
}

// Cas où la pioche presente 3 mêmes JetonFaune : dans ce cas, il est tolerer de les remplacer UNE FOIS
// Cas où coup Jeton Nature : le joeur selectionne les jetons faunes qu'ils veut remplacer
void Pioche::resetJetonFaune(const std::vector<int>& indices) {
	for (int i : indices) {
		if (i >= 0 && i < MAX) {
			if (visibilite.at(i).second) {
				retirerJetonVisible(i, true); //remet dans la pioche
			}
		}
		else {
			throw std::out_of_range("Indice hors intervalle pour retirer le jeton Faune");
		}
	}
	rafraichirPioche();
}

// retire une paire fixe : coup "normal"
// ne pas remettre la Tuile et le JetonFaune dans la pioche, car le joueur en sera en possession
void Pioche::retirerPaire(unsigned int i) {
	checkIndex(i);
	retirerTuileVisible(i);
	retirerJetonVisible(i); 
}

// retire une paire libre : coup "Jeton Nature"
// ne pas remettre la Tuile et le JetonFaune dans la pioche, car le joueur en sera en possession
void Pioche::retirerPaire(unsigned int i, unsigned int j) {
	checkIndex(i);
	checkIndex(j);
	retirerTuileVisible(i);
	retirerJetonVisible(j);
}

// rend invisible une Tuile 
void Pioche::retirerTuileVisible(unsigned int indexTuile)
{
	checkIndex(indexTuile);
	visibilite.at(indexTuile).first = false;
}

// rend invisible un JetonFaune
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

// TODO : review pending
// exe : removePair() + slideApresJetonNature() + Rafraichir()
//void Pioche::slideApresJetonNature(int i, bool isTuile) {
//	// Condition de base : si i est inf�rieur � 1, on arr�te la r�cursion
//	if (i < 1) {
//		return;
//	}
//	// V�rifie si la visibilit� est vraie
//	if (isTuile) {
//		if (visibilite[i].first) {
//			slideApresJetonNature(i - 1, isTuile); // Appel r�cursif pour l'indice pr�c�dent
//		}
//		else {
//			// On glisse en arri�re pour trouver le premier indice visible
//			for (int j = i; j >= 0; --j) {
//				if (visibilite[j].first) {
//					// On utilise une variable temporaire pour l'�change
//					auto& tmp = pioche[i]->first; // On garde l'�l�ment actuel dans une variable temporaire
//					pioche[i]->first = pioche[j]->first; // Remplace la tuile
//					pioche[j]->first = tmp; // D�place l'�l�ment original � l'indice j
//
//					// Met � jour la visibilit�
//					visibilite[i].first = true; // Met � jour la visibilit� pour l'indice actuel
//					visibilite[j].first = false; // Met � jour la visibilit� de l'indice original
//					break; // On sort de la boucle apr�s le glissement
//				}
//			}
//			slideApresJetonNature(i - 1, isTuile); // On continue avec l'indice pr�c�dent
//		}
//	}
//	else {
//		if (visibilite[i].second) {
//			slideApresJetonNature(i - 1, isTuile); // Appel r�cursif pour l'indice pr�c�dent
//		}
//		else {
//			// On glisse en arri�re pour trouver le premier indice visible
//			for (int j = i; j >= 0; --j) {
//				if (visibilite[j].second) {
//					// On utilise une variable temporaire pour l'�change
//					auto& tmp = pioche[i]->second; // On garde l'�l�ment actuel dans une variable temporaire
//					pioche[i]->second = pioche[j]->second; // Remplace la tuile
//					pioche[j]->second = tmp; // D�place l'�l�ment original � l'indice j
//
//					// Met � jour la visibilit�
//					visibilite[i].second = true; // Met � jour la visibilit� pour l'indice actuel
//					visibilite[j].second = false; // Met � jour la visibilit� de l'indice original
//					break; // On sort de la boucle apr�s le glissement
//				}
//			}
//			slideApresJetonNature(i - 1, isTuile); // On continue avec l'indice pr�c�dent
//		}
//	}
//}

//void Pioche::slideTuile(int i) {
//	slideApresJetonNature(i, true);
//}
//
//void Pioche::slideJeton(int i) {
//	slideApresJetonNature(i, false); 
//}



// simule un robot qui retire une paire en cas de joueur solitaire
void Pioche::retirePaireExtreme() {
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

std::ostream& operator<<(std::ostream& os, const Pioche& p) {
	os << "PIOCHE :\n";
	for (unsigned int i = 0; i < p.getPiocheVisible().size(); ++i) {
		os << "Index " << i << ": ";
		try {
			auto [tuile, jeton] = p.getPaire(i);
			os << "Tuile : " << tuile << ", JetonFaune : " << jeton;
		}
		catch (const std::logic_error& e) {
			os << "Erreur : " << e.what();
		}
		os << '\n';
	}
	return os;
}