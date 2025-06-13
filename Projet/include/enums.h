#pragma once
#include <string>
#include <vector>
#include <ostream>
#include <vector>
#include "json.hpp"

enum class Habitat { marais, fleuve, montagne, prairie, foret };
enum class Faune { saumon = 0, ours = 1, buse = 2, renard = 3, wapiti = 4, rien = -1};
enum class Direction { NordEst = 0, Est = 1, SudEst = 2, SudOuest = 3, Ouest = 4, NordOuest = 5, Inconnue = -1 };
enum class Marquage { A, B, C, D, famille, intermediaire };
enum class Variante { standard, famille, intermediaire };

std::string habitatToString(Habitat habitat);
std::string fauneToString(Faune faune);
std::string directionToString(Direction dir);
std::string directionToStringSigle(Direction dir);
Direction stringToDirection(const std::string s);
Faune stringToFaune(const std::string& s);
Habitat stringToHabitat(const std::string& s);
bool stringToBool(const std::string& str);
Marquage stringToMarquage(const std::string& s);
Variante stringToVariante(const std::string& s);
std::string marquageToString(Marquage m);
std::string varianteToString(Variante v);

std::ostream& operator<<(std::ostream& flux, Habitat h);
std::ostream& operator<<(std::ostream& flux, Faune f);
std::ostream& operator<<(std::ostream& flux, Direction d);
std::ostream& operator<<(std::ostream& flux, std::vector<Direction> dirs);


NLOHMANN_JSON_SERIALIZE_ENUM(Habitat, {
    {Habitat::marais, "marais"},
    {Habitat::fleuve, "fleuve"},
    {Habitat::montagne, "montagne"},
    {Habitat::prairie, "prairie"},
    {Habitat::foret, "foret"},
    })

NLOHMANN_JSON_SERIALIZE_ENUM(Faune, {
    {Faune::rien, nullptr}, 
    {Faune::saumon, "saumon"},
    {Faune::ours, "ours"},
    {Faune::buse, "buse"},
    {Faune::renard, "renard"},
    {Faune::wapiti, "wapiti"},
    })

NLOHMANN_JSON_SERIALIZE_ENUM(Direction, {
    {Direction::Inconnue, nullptr},
    {Direction::NordEst, "NordEst"},
    {Direction::Est, "Est"},
    {Direction::SudEst, "SudEst"},
    {Direction::SudOuest, "SudOuest"},
    {Direction::Ouest, "Ouest"},
    {Direction::NordOuest, "NordOuest"},
    })

NLOHMANN_JSON_SERIALIZE_ENUM(Marquage, {
    {Marquage::A, "A"},
    {Marquage::B, "B"},
    {Marquage::C, "C"},
    {Marquage::D, "D"},
    {Marquage::famille, "famille"},
    {Marquage::intermediaire, "intermediaire"},
    })

NLOHMANN_JSON_SERIALIZE_ENUM(Variante, {
    {Variante::standard, "standard"},
    {Variante::famille, "famille"},
    {Variante::intermediaire, "intermediaire"},
    })


