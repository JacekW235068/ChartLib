#include <ChartLib/PlotData.hpp>
#include <ChartLib/Plot.hpp>

#include <memory>

namespace chart {

PlotData::PlotData (char Symbol,std::string Name, Style Style, Color Color) 
: color(Color), name(Name), symbol(Symbol), min_x(nan("")), max_x(nan("")), min_y(nan("")), max_y(nan("")), style(Style)
{
    setStyledSymbol();
}

// TODO: This whol shit has to go, switch to weak_ptr
PlotData::~PlotData(){}

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