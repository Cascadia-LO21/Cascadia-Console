#pragma once
#include <ostream>
#include "jeton_faune.h"
#include "position.h"
#include "pioche.h"
#include "env_joueur.h"
#include "partie.h"

std::ostream& operator<<(std::ostream& os, const JetonFaune& j);

std::ostream& operator<<(std::ostream& flux, const Position& p);
std::ostream& operator<<(std::ostream& flux, const std::vector<Position>& v);

std::ostream& operator<<(std::ostream& flux, const Tuile& tuile);
std::ostream& operator<<(std::ostream& flux, const std::vector<Tuile>& tuileDepart);

std::ostream& operator<<(std::ostream& os, const Pioche& p);

std::ostream& operator<<(std::ostream& os, const EnvJoueur& env);

//std::ostream& operator<<(std::ostream& os, const Partie& partie);

void affichePioche(const Partie& p);
void afficheJoueurs(const Partie& p);
void afficheEnvJoueurCourant(const Partie& p);

void afficherMessageBienvenu();
void afficherMarquage();
void afficherVariante();
void afficherMessagePret();
void afficherTour(Partie& p);
void afficherMenuJetonNature();
void afficherMessageFin();

