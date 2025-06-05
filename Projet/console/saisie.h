#pragma once
#include "enums.h"
#include "env_joueur.h"
#include "position.h"
#include "partie.h"
#include "jeton_faune.h"

void saisirJoueur(unsigned int max, std::vector<EnvJoueur>& joueurs);

unsigned int saisirNombre(unsigned int max);

const Position saisirPositionTuile(Partie p);

const Position saisirPositionJeton(Partie p, Faune f);