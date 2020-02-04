#pragma once
#include "../Enum/Style.cpp"
#include <list>
#include <tuple>
#include <cmath>

class PlotData
{
public: 
    //line,dots, that kinda stuff
    Style style;
    //Symbol to represent data on chart
    char symbol;
    virtual std::tuple<double,double,double,double> getRange() const = 0;
    virtual std::list<std::pair<double,double>> getData() const = 0;
    virtual std::list<std::pair<double,double>> getData(std::pair<double,double> Xrange, std::pair<double,double> Yrange) const = 0;
    PlotData(char Symbol = 'o', Style Style = Style::dots);
};
 