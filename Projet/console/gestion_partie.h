#pragma once
#include <ostream>
#include "jeton_faune.h"
#include "position.h"
#include "pioche.h"
#include "env_joueur.h"
#include "partie.h"

void gererJetonsIdentiques(Partie& p);

void jouerTour(Partie& p);

void gererJetonsIndesirables(Partie& p);

bool placerTuileEtJeton(Partie& p, EnvJoueur& player, 
	unsigned int rep, bool jetonNatureUsed);

void gestionPause(Partie& p);