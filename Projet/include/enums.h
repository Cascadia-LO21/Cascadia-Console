#pragma once

#include <string>
#include <ostream>

enum class Habitat { marais, fleuve, montagne, prairie, foret };
enum class Faune { saumon, ours, buse, renard, wapiti, rien };
enum class Direction { NordEst = 0, Est = 1, SudEst = 2, SudOuest = 3, Ouest = 4, NordOuest = 5, Inconnue = -1 };

std::string habitatToString(Habitat habitat);
std::string fauneToString(Faune faune);
Faune stringToFaune(const std::string& s);
std::string directionToString(Direction dir);

std::ostream& operator<<(std::ostream& flux, Habitat h);
std::ostream& operator<<(std::ostream& flux, Faune f);
std::ostream& operator<<(std::ostream& flux, Direction d);

