#include <iostream>
#include "jeton_faune.h"
#include "enums.h"

std::ostream& operator<<(std::ostream& os, const JetonFaune& j) {
	os << fauneToString(j.getType());
	return os;
}