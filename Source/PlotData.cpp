#include <PlotData.hpp>
#include <Plot.hpp>

#include <memory>

namespace chart {

PlotData::PlotData (char Symbol,std::string Name, Style Style, Color Color) 
: symbol(Symbol), style(Style), color(Color), min_x(nan("")), max_x(nan("")), min_y(nan("")), max_y(nan("")), name(Name)
{
    setStyledSymbol();
}

PlotData::~PlotData(){
    for(Plot* plot : plots){
        auto it = plot->ChartMap.begin();
	    while (it != plot->ChartMap.end())
	    {
            if ((it->second) == &getStyledSymbol())
            {
                it = plot->ChartMap.erase(it);
            }
            else {
                ++it;
            }
	    }
        plot->dataSets.remove(this);
    }
}

    std::string PlotData::mapColor(Color color){
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
            default: 
                return "\033[39m";
        }
    }
    const char& PlotData::getSymbol() const{
        return symbol;
    }
    void PlotData::setSymbol(char Symbol){
        symbol = Symbol;
        setStyledSymbol();
    }
    const Color& PlotData::getColor() const{
        return color;
    }
    void PlotData::setColor(Color Color){
        color = Color;
        setStyledSymbol();
    }
    const std::string& PlotData::getStyledSymbol() const{
        return styledSymbol;
    }
    void PlotData::setStyledSymbol(){
        styledSymbol = mapColor(color) + symbol;
    }

const std::string& PlotData::getName() const{
    return name;
}
void PlotData::setName(std::string Name){
    name = Name;
}
}