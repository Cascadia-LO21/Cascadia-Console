#pragma once
#include "enums.h"
#include "env_joueur.h"
#include "position.h"
#include "partie.h"
#include "jeton_faune.h"

unsigned int saisirNombre(unsigned int max);

const Position saisirPositionTuile(const Partie& p);

const Position saisirPositionJeton(const Partie& p, Faune f);