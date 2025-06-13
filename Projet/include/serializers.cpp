#include "tuile.h"
#include "position.h"
#include "env_joueur.h" 
#include "json.hpp"
#include <unordered_map>
#include <unordered_set>

// Sérialiseur unique_ptr (à mettre UNE SEULE FOIS dans le projet)
namespace nlohmann {
    template <typename T>
    struct adl_serializer<std::unique_ptr<T>> {
        static void to_json(json& j, const std::unique_ptr<T>& opt) {
            if (opt) j = *opt; else j = nullptr;
        }
        static void from_json(const json& j, std::unique_ptr<T>& opt) {
            if (j.is_null()) opt.reset();
            else opt = std::make_unique<T>(j.get<T>());
        }
    };
}

// Sérialiseur unordered_map<Position, Tuile>
namespace nlohmann {
    template <>
    struct adl_serializer<std::unordered_map<Position, Tuile>> {
        static void to_json(json& j, const std::unordered_map<Position, Tuile>& map) {
            j = json::array();
            for (const auto& [pos, tuile] : map)
                j.push_back({ {"position", pos}, {"tuile", tuile} });
        }
        static void from_json(const json& j, std::unordered_map<Position, Tuile>& map) {
            map.clear();
            for (const auto& item : j) {
                Position pos = item.at("position").get<Position>();
                Tuile tuile = item.at("tuile").get<Tuile>();
                map[pos] = tuile;
            }
        }
    };
}

// Sérialiseur unordered_set<Position>
namespace nlohmann {
    template <>
    struct adl_serializer<std::unordered_set<Position>> {
        static void to_json(json& j, const std::unordered_set<Position>& set) {
            j = json::array();
            for (const auto& pos : set) j.push_back(pos);
        }
        static void from_json(const json& j, std::unordered_set<Position>& set) {
            set.clear();
            for (const auto& item : j) set.insert(item.get<Position>());
        }
    };
}

// Sérialiseur unordered_map<Faune, unordered_set<Position>>
namespace nlohmann {
    template <>
    struct adl_serializer<std::unordered_map<Faune, std::unordered_set<Position>>> {
        static void to_json(json& j, const std::unordered_map<Faune, std::unordered_set<Position>>& map) {
            j = json::array();
            for (const auto& [faune, positions] : map)
                j.push_back({ {"faune", faune}, {"positions", positions} });
        }
        static void from_json(const json& j, std::unordered_map<Faune, std::unordered_set<Position>>& map) {
            map.clear();
            for (const auto& item : j) {
                Faune faune = item.at("faune").get<Faune>();
                std::unordered_set<Position> positions = item.at("positions").get<std::unordered_set<Position>>();
                map[faune] = positions;
            }
        }
    };
}
