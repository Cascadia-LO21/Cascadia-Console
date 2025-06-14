#pragma once

#include <ostream>
#include "enums.h"

class JetonFaune {
	Faune type;
public:
	JetonFaune(Faune type) : type(type) {}
	Faune getType() const { return type; };
	// constructeur par defaut pour Pioche
	JetonFaune() : type(Faune::rien) {}
	void setType(Faune t) { type = t; }

};

