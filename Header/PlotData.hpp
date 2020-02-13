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
    //cached range
    mutable double min_x;
    mutable double min_y;
    mutable double max_x;
    mutable double max_y;
    virtual std::tuple<double,double,double,double> getRange() const = 0;
    virtual std::list<std::pair<double,double>>& getData() = 0;
    virtual std::list<std::pair<double,double>> getData(std::pair<double,double> Xrange, std::pair<double,double> Yrange) const = 0;
    PlotData(char Symbol = 'o', Style Style = Style::dots);
    PlotData(const PlotData& plot) = delete; //not sure if it does anything but just keeping it as temporary check for copying
};
 