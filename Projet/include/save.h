#pragma once
#include "enums.h"
#include "position.h"
#include "tuile.h"
#include "jeton_faune.h"
#include "env_joueur.h"
#include "pioche.h"
#include "partie.h"
#include <fstream>


template<typename Enum>
void saveEnum(std::ostream& os, Enum e);
template<typename Enum>
void loadEnum(std::istream& is, Enum& e);

void save(std::ostream& os, const Position& pos);
void load(std::istream& is, Position& pos);
void save(std::ostream& os, const JetonFaune& jf);
void load(std::istream& is, JetonFaune& jf);

void save(std::ostream& os, const Tuile& t);
void load(std::istream& is, Tuile& t);

void save(std::ostream& os, const Tuile& t);
void load(std::istream& is, Tuile& t);

void save(std::ostream& os, const Pioche& p);

void save(std::ostream& os, const Partie& partie);
void load(std::istream& is, Partie& partie);