#pragma once
//#include <string>
#include "enums.h"
#include "env_joueur.h"
#include "position.h"
#include "partie.h"
#include "jeton_faune.h"

unsigned int saisirNombre(unsigned int max);

bool saisirReponse();

const Position saisirPositionTuile(const Partie& p);

const std::optional<Position> saisirPositionJeton(const Partie& p, Faune f);

Position saisirCoordonnees(const EnvJoueur& j);

void saisirJoueurs(Partie& p);

Direction saisirDirection(const std::vector<Direction>& dirValides, const Partie& p, const Position& pos);

std::string toLowerNoSpace(const std::string& s);