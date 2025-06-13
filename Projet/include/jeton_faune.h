#pragma once

#include <ostream>
#include "enums.h"
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

class JetonFaune {
	Faune type;
public:

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(JetonFaune, type)

	JetonFaune(Faune type) : type(type) {}
	Faune getType() const { return type; };
	// constructeur par defaut pour Pioche
	JetonFaune() : type(Faune::rien) {}
};

