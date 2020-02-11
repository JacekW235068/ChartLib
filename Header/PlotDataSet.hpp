#pragma once
#include "../Enum/Style.cpp"
#include <vector>
#include <tuple>
#include <cmath>
#include <functional>

#include "./PlotData.hpp"

class PlotDataSet : public PlotData
{
private:
    std::list<std::pair<double, double>> dataSet;
public:
    PlotDataSet(std::list<std::pair<double, double>> DataSet, char Symbol = 'o', Style Style = Style::dots);
    std::tuple<double,double,double,double> getRange() const;
    std::list<std::pair<double,double>>& getData();
    std::list<std::pair<double,double>> getData(std::pair<double,double> Xrange, std::pair<double,double> Yrange) const;
};  
