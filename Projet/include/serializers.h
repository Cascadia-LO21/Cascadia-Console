#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "json.hpp"
#include "enums.h"
#include "position.h"
#include "tuile.h"


// pour sauvegarder les pointeurs intelligents unique_ptr
namespace nlohmann {
    template <typename T>
    struct adl_serializer<std::unique_ptr<T>> {
    };

    template <>
    struct adl_serializer<std::unordered_map<Position, Tuile>> {

    };

    template <>
    struct adl_serializer<std::unordered_set<Position>> {

    };

    template <>
    struct adl_serializer<std::unordered_map<Faune, std::unordered_set<Position>>> {
    };
}
