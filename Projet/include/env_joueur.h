#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <ostream>
#include "position.h"
#include "tuile.h"
#include "jeton_faune.h"


class EnvJoueur {
	std::string pseudo;
	size_t nbJetonNature;
	size_t scoreFinal;

public:
	
};

std::ostream& operator<<(std::ostream& os, const EnvJoueur& e);