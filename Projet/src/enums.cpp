#include <stdexcept>
#include <vector>
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

Marquage stringToMarquage(const std::string& s) {
	if (s == "A" || s == "a")	return Marquage::A;
	if (s == "B" || s == "b")	return Marquage::B;
	if (s == "C" || s == "c")	return Marquage::C;
	if (s == "D" || s == "d")	return Marquage::D;
	if (s == "famille")			return Marquage::famille;
	if (s == "intermediaire")	return Marquage::intermediaire;
	throw std::invalid_argument("Marquage inconnu: " + s);
}

Variante stringToVariante(const std::string& s) {
	if (s == "standard")	return Variante::standard;
	if (s == "famille")		return Variante::famille;
	if (s == "intermediaire")	return Variante::intermediaire;
	throw std::invalid_argument("Variante inconnue : " + s);
}

std::string marquageToString(Marquage m) {
	switch (m)
	{
	case Marquage::A:	return "A";
	case Marquage::B:	return "B";
	case Marquage::C:	return "C";
	case Marquage::D:	return "D";
	case Marquage::famille: return "famille";
	case Marquage::intermediaire: return "intermediaire";
	default:			
			return "A";
	}
}

std::string varianteToString(Variante v){
	switch (v)
	{
	case Variante::standard:		return "standard";
	case Variante::famille:			return "famille";
	case Variante::intermediaire:	return "intermediaire";
	default:
		return "standar";
	}
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

// = 0, Est = 1, SudEst = 2, SudOuest = 3, Ouest = 4, NordOuest = 5
Direction stringToDirection(const std::string s)
{
	if (s == "NE" || s == "ne")	return Direction::NordEst;
	if (s == "E" || s == "e")	return Direction::Est;
	if (s == "SE" || s == "se")	return Direction::SudEst;
	if (s == "SO" || s == "so")	return Direction::SudOuest;
	if (s == "O" || s == "o")	return Direction::Ouest;
	if (s == "NO" || s == "no")	return Direction::NordOuest;
	throw std::invalid_argument("Direction inconnue : " + s);
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

std::ostream& operator<<(std::ostream& flux, std::vector<Direction> dirs) {
	for (const Direction& d : dirs) {
		flux << directionToString(d) << "  ";
	}
	return flux;
}