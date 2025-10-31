#include "GlobalVar.hpp"

namespace obl{
    std::unordered_map<std::string, std::string> Sprites;
    std::unordered_map<std::string, std::string> Fonts;
    std::vector<std::shared_ptr<window>> Windows;

    int PixelsPerUnit = 35;
}