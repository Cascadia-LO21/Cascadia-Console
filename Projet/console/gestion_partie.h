#pragma once
#include <ostream>
#include <fstream>
#include <cstdio>
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
bool abandonner(const Partie& p);
bool sauvegarder(const Partie& p, const std::string& f = "sauvegarde.json");

void choisirVariante(Partie& p);
void choisirMarquage(Partie& p);

void sauvegarderPartie(const Partie& p, const std::string& f = "sauvegarde.json");
bool chargerPartie(Partie& p, const std::string& f = "sauvegarde.json");
void supprimerSauvegarde(const std::string& f = "sauvegarde.json");
bool proposerReprisePartie(Partie& p, const std::string& f = "sauvegarde.json");

bool parametrage(Partie& p, const std::string& f = "sauvegarde.json");