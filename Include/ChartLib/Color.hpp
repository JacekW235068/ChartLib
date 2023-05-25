#pragma once
#include <string>

namespace chart{

enum class Color{
GREY,
RED ,
GREEN ,
YELLOW ,
BLUE ,
PINK ,
DARK_GREY ,
DARK_RED ,
DARK_GREEN ,
ORANGE ,
DARK_BLUE ,
DARK_PINK,
none
};

inline std::string mapColor(Color color){
    switch(color){
        case Color::RED:
            return "\033[91m";
        case Color::GREEN:
            return "\033[92m";
        case Color::BLUE:
            return "\033[94m";
        case Color::GREY:
            return "\033[90m";
        case Color::YELLOW:
            return "\033[93m";
        case Color::PINK:
            return "\033[95m";
        case Color::DARK_GREY:
            return "\033[30m";
        case Color::DARK_RED:
            return "\033[31m";
        case Color::DARK_GREEN:
            return "\033[32m";
        case Color::ORANGE:
            return "\033[33m";
        case Color::DARK_BLUE:
            return "\033[34m";
        case Color::DARK_PINK:
            return "\033[35m";
        case Color::none:
            return "\033[39m";
        default: 
            return "";
    }
}
}