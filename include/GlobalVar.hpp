#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

#include "GlobalTypes.hpp"

namespace obl{
    struct window;

    extern std::unordered_map<std::string, std::string> Sprites;
    extern std::unordered_map<std::string, std::string> Fonts;
    extern std::vector<std::shared_ptr<window>> Windows;

    extern int PixelsPerUnit;
}