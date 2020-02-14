#include "../Header/PlotData.hpp"
#include "../Header/Plot.hpp"

PlotData::PlotData (char Symbol, Style Style) 
: symbol(Symbol), style(Style), min_x(nan("")), max_x(nan("")), min_y(nan("")), max_y(nan(""))
{}

PlotData::~PlotData(){
}



