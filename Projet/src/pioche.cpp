#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <stdexcept>
#include "pioche.h"
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

void Pioche::setTuilesVisibles(unsigned int i, bool vue) {
	if (i >= MAX)
		throw std::out_of_range("L'indice maximal supporté est : " + std::to_string(MAX));
	
	if (tuilesVisibles.at(i) != vue) {
		tuilesVisibles.at(i) = vue;
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

// les methodes de la classe Pioche
void Pioche::resetAllJetonFaune(std::stack<Tuile>& pileTuiles, std::vector<JetonFaune>& sachetJetons) {
	removeAllJetonFaune();
	rafraichirPioche(pileTuiles, sachetJetons);
}

void Pioche::removeAllJetonFaune() {
	for (int i = 0; i < pioche.size(); ++i) {
		pioche[i]->second = std::nullopt;
		retirerJetonVisible(i);
	}
}

void Pioche::resetJetonFaune(std::stack<Tuile>& pileTuiles, std::vector<JetonFaune>& sachetJetons, const std::vector<int>& quiEnleverIndices) {
	removeJetonFaune(quiEnleverIndices);
	rafraichirPioche(pileTuiles, sachetJetons);
}

void Pioche::removeJetonFaune(const std::vector<int>& quiEnleverIndices) {
	for (int indice : quiEnleverIndices) {
		if (indice >= 0 && indice < pioche.size()) {
			if (pioche[indice]) {
				pioche[indice]->second = std::nullopt;
				retirerJetonVisible(indice);
			}
		}
		else {
			throw std::out_of_range("Indice hors intervalle pour retirer le jeton Faune");
		}
	}
}

void Pioche::removePair(unsigned int indiceTuile, unsigned int indiceJetonFaune) {
	// verification des parametres en entree
	if (indiceTuile >= pioche.size()) {
		throw std::out_of_range("Indice de tuile hors intervalle");
	}
	if (indiceJetonFaune >= pioche.size()) {
		throw std::out_of_range("Indice de jeton Faune hors intervalle");
	}

	// verification d'existences
	if (pioche[indiceTuile] && pioche[indiceJetonFaune]) {
		pioche[indiceTuile]->first = std::nullopt; // Retire la Tuile
		pioche[indiceTuile]->second = std::nullopt; // Retire le JetonFaune
		retirerTuileVisible(indiceTuile);
		retirerJetonVisible(indiceJetonFaune);
	}
	else {
		throw std::logic_error("Aucune paire disponible � l'indice de tuile donn�");
	}
}

void Pioche::removeLastPair() {
	if (pioche[3]) {
		// retirer paire indice = 3
		pioche[3] = std::nullopt; // optional

		// MAJ visibilite
		retirerTuileVisible(3);
		retirerJetonVisible(3);
	}
	else {
		throw std::logic_error("Aucune paire � retirer � l'indice 3");
	}
}


void Pioche::retirerPaire(unsigned int indice) {
	if (indice < 0 || indice >= pioche.size()) {
		throw std::out_of_range("Indice hors intervalle pour retirer la paire");
	}

	if (pioche[indice]) {
		pioche[indice] = std::nullopt;

		retirerTuileVisible(indice);
		retirerJetonVisible(indice);
	}
	else {
		throw std::logic_error("Aucune paire disponible � cet indice");
	}
}

void Pioche::retirerTuileVisible(unsigned int indexTuile)
{
	if (indexTuile < visibilite.size()) {
		visibilite[indexTuile].first = false;
	}
	else {
		throw std::out_of_range("Indice hors intervalle pour retirer la tuile visible");
	}
}

void Pioche::retirerJetonVisible(unsigned int indexJeton)
{
	if (indexJeton < visibilite.size()) {
		visibilite[indexJeton].second = false;
	}
	else {
		throw std::out_of_range("Indice hors intervalle pour retirer le jeton visible");
	}
}

bool Pioche::jetonsIdentiques(int nb) const {
	std::array<int, 5> fauneCount{ 0 };
	for (const auto& optionalPair : pioche) {
		if (optionalPair) {
			const auto& jetonFaune = optionalPair->second;
			if (jetonFaune) {
				fauneCount[retourneIndiceFaune(jetonFaune->getType())]++;
			}
		}
	}
	for (const auto& value : fauneCount) {
		if (value == nb) {
			return true;
		}
	}
	return false;
}

int Pioche::retourneIndiceFaune(Faune type) const
{
	switch (type) {
	case Faune::saumon: return 0;
	case Faune::ours:   return 1;
	case Faune::buse:   return 2;
	case Faune::renard: return 3;
	case Faune::wapiti: return 4;
	default: throw std::invalid_argument("Type de Faune Inconnu");
	}
}

bool Pioche::quatreJetonsIdentiques() const {
	return jetonsIdentiques(4);
}

bool Pioche::troisJetonsIdentiques() const {
	return jetonsIdentiques(3);
}

// exe : removePair() + slideApresJetonNature() + Rafraichir()
void Pioche::slideApresJetonNature(int i, bool isTuile) {
	// Condition de base : si i est inf�rieur � 1, on arr�te la r�cursion
	if (i < 1) {
		return;
	}
	// V�rifie si la visibilit� est vraie
	if (isTuile) {
		if (visibilite[i].first) {
			slideApresJetonNature(i - 1, isTuile); // Appel r�cursif pour l'indice pr�c�dent
		}
		else {
			// On glisse en arri�re pour trouver le premier indice visible
			for (int j = i; j >= 0; --j) {
				if (visibilite[j].first) {
					// On utilise une variable temporaire pour l'�change
					auto& tmp = pioche[i]->first; // On garde l'�l�ment actuel dans une variable temporaire
					pioche[i]->first = pioche[j]->first; // Remplace la tuile
					pioche[j]->first = tmp; // D�place l'�l�ment original � l'indice j

					// Met � jour la visibilit�
					visibilite[i].first = true; // Met � jour la visibilit� pour l'indice actuel
					visibilite[j].first = false; // Met � jour la visibilit� de l'indice original
					break; // On sort de la boucle apr�s le glissement
				}
			}
			slideApresJetonNature(i - 1, isTuile); // On continue avec l'indice pr�c�dent
		}
	}
	else {
		if (visibilite[i].second) {
			slideApresJetonNature(i - 1, isTuile); // Appel r�cursif pour l'indice pr�c�dent
		}
		else {
			// On glisse en arri�re pour trouver le premier indice visible
			for (int j = i; j >= 0; --j) {
				if (visibilite[j].second) {
					// On utilise une variable temporaire pour l'�change
					auto& tmp = pioche[i]->second; // On garde l'�l�ment actuel dans une variable temporaire
					pioche[i]->second = pioche[j]->second; // Remplace la tuile
					pioche[j]->second = tmp; // D�place l'�l�ment original � l'indice j

					// Met � jour la visibilit�
					visibilite[i].second = true; // Met � jour la visibilit� pour l'indice actuel
					visibilite[j].second = false; // Met � jour la visibilit� de l'indice original
					break; // On sort de la boucle apr�s le glissement
				}
			}
			slideApresJetonNature(i - 1, isTuile); // On continue avec l'indice pr�c�dent
		}
	}
}

void Pioche::slideTuile(int i) {
	slideApresJetonNature(i, true);
}

void Pioche::slideJeton(int i) {
	slideApresJetonNature(i, false); 
}

void Pioche::remplacerJetons(std::stack<Tuile>& pileTuiles, std::vector<JetonFaune>& sachetJetons, int except) {
	if (except == -1) {
		removeAllJetonFaune(); 
	}
	else if (except >= 0 && except < pioche.size()) {
		std::vector<int> indicesToRemove;
		for (int i = 0; i < pioche.size(); ++i) {
			if (i != except) {
				indicesToRemove.push_back(i);
			}
		}
		removeJetonFaune(indicesToRemove); 
	}
	else {
		throw std::out_of_range("Indice hors intervalle pour remplacer les jetons");
	}

	rafraichirPioche(pileTuiles, sachetJetons); 
}

void Pioche::rafraichirPioche(std::stack<Tuile>& pileTuiles, std::vector<JetonFaune>& sachetJetons) {
	for (unsigned int i = 0; i < pioche.size(); ++i) {
		auto [ancienTuile, ancienJeton] = getPaire(i);
		if (!visibilite[i].first) {
			if (!pileTuiles.empty()) {
				Tuile nouvelleTuile = GestionPieces::piocherTuile(pileTuiles);
				visibilite[i].first = true;
				setPaire(i, nouvelleTuile, ancienJeton);
			}
		}
		if (!visibilite[i].second) {
			if (!sachetJetons.empty()) {
				JetonFaune nouveauJeton = GestionPieces::piocherJeton(sachetJetons);
				visibilite[i].second = true;
				setPaire(i, ancienTuile, nouveauJeton);
			}
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Pioche& p) {
	os << "Pioche :\n";
	for (unsigned int i = 0; i < p.getPioche().size(); ++i) {
		os << "Index " << i << ": ";
		try {
			auto [tuile, jeton] = p.getPaire(i); // Utilisation de getPaire
			os << "Tuile : ";
			if (tuile) {
				os << *tuile; // Utilisation de l'op�rateur << pour Tuile
			}
			else {
				os << "Aucune";
			}
			os << ", JetonFaune : ";
			if (jeton) {
				os << *jeton; // Utilisation de l'op�rateur << pour JetonFaune
			}
			else {
				os << "Aucun";
			}
		}
		catch (const std::logic_error& e) {
			os << "Erreur : " << e.what(); // Gestion des erreurs
		}
		os << '\n'; // Utilisation de '\n' pour une meilleure performance
	}
	return os;
}