#pragma once

#include <string>
#include <ostream>

enum class Habitat { marais, fleuve, montagne, prairie, foret };
enum class Faune { saumon, ours, buse, renard, wapiti, rien };
enum class Direction { NordEst = 0, Est = 1, SudEst = 2, SudOuest = 3, Ouest = 4, NordOuest = 5, Inconnue = -1 };
enum class Marquage { A, B, C, D, famille, intermediaire };
enum class Variante { standard, famille, intermediaire };

std::string habitatToString(Habitat habitat);
std::string fauneToString(Faune faune);
std::string directionToString(Direction dir);
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

