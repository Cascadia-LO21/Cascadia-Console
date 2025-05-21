#include <stdexcept>
#include "enums.h"

std::string habitatToString(Habitat habitat) {
	switch (habitat) {
	case Habitat::marais:       return "marais";
	case Habitat::fleuve:		return "fleuve";
	case Habitat::montagne:		return "montagne";
	case Habitat::prairie:		return "prairie";
	case Habitat::foret:		return "foret";
	default:                    return "rien";
	}
}

std::string fauneToString(Faune faune) {
	switch (faune) {
	case Faune::saumon:			return "saumon";
	case Faune::ours:			return "ours";
	case Faune::buse:			return "buse";
	case Faune::renard:			return "renard";
	case Faune::wapiti:			return "wapiti";
	case Faune::rien:			return "rien";
	default:                    return "rien";
	}
}

Faune stringToFaune(const std::string& s) {
	if (s == "saumon") return Faune::saumon;
	if (s == "ours")   return Faune::ours;
	if (s == "buse")   return Faune::buse;
	if (s == "renard") return Faune::renard;
	if (s == "wapiti") return Faune::wapiti;
	if (s == "rien")	return Faune::rien;
	throw std::invalid_argument("Faune inconnu : " + s);
}

Habitat stringToHabitat(const std::string& s) {
	if (s == "marais")		return Habitat::marais;
	if (s == "fleuve")		return Habitat::fleuve;
	if (s == "montagne")	return Habitat::montagne;
	if (s == "prairie")		return Habitat::prairie;
	if (s == "forêt")		return Habitat::foret;
	throw std::invalid_argument("Habitat inconnu: " + s);
}

bool stringToBool(const std::string& str) {
	return str == "true";
}

std::string directionToString(Direction dir) {
	switch (dir) {
	case Direction::Est:       return "Est";
	case Direction::NordEst:   return "NordEst";
	case Direction::NordOuest: return "NordOuest";
	case Direction::Ouest:     return "Ouest";
	case Direction::SudOuest:  return "SudOuest";
	case Direction::SudEst:    return "SudEst";
	default:                   return "Inconnu";
	}
}

std::ostream& operator<<(std::ostream& flux, Habitat h) {
	flux << habitatToString(h);
	return flux;
}

std::ostream& operator<<(std::ostream& flux, Faune f) {
	flux << fauneToString(f);
	return flux;
}

std::ostream& operator<<(std::ostream& flux, Direction d) {

	flux << directionToString(d);
	return flux;
}