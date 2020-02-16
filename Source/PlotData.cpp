#include "../Header/PlotData.hpp"
#include "../Header/Plot.hpp"

PlotData::PlotData (char Symbol, Style Style, Color Color) 
: symbol(Symbol), style(Style), color(Color), min_x(nan("")), max_x(nan("")), min_y(nan("")), max_y(nan(""))
{
    setStyledSymbol();
}

PlotData::~PlotData(){
}

    std::string PlotData::mapColor(Color color){
        switch(color){
            case Color::RED:
                return "\033[91m";
            case Color::GREEN:
                return "\033[92m";
            case Color::BLUE:
                return "\033[94m";
            default: 
                return "\033[39m";
        }
    }
    char PlotData::getSymbol(){
        return symbol;
    }
    void PlotData::setSymbol(char Symbol){
        symbol = Symbol;
        setStyledSymbol();
    }
    Color PlotData::getColor(){
        return color;
    }
    void PlotData::setColor(Color Color){
        color = Color;
        setStyledSymbol();
    }
    const std::string& PlotData::getStyledSymbol(){
        return styledSymbol;
    }
    void PlotData::setStyledSymbol(){
        styledSymbol = mapColor(color) + symbol;
    }
