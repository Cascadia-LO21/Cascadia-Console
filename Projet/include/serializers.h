#pragma once
#include <memory>
#include "json.hpp"

// pour sauvegarder les pointeurs intelligents unique_ptr
namespace nlohmann {
    template <typename T>
    struct adl_serializer<std::unique_ptr<T>> {
        static void to_json(json& j, const std::unique_ptr<T>& opt) {
            if (opt) {
                j = *opt;
            }
            else {
                j = nullptr;
            }
        }
        static void from_json(const json& j, std::unique_ptr<T>& opt) {
            if (j.is_null()) {
                opt.reset();
            }
            else {
                opt = std::make_unique<T>(j.get<T>());
            }
        }
    };
}
