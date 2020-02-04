#pragma once
#include "../Enum/Style.cpp"
#include <vector>
#include <tuple>
#include <cmath>

#include "./PlotData.hpp"

class PlotDataSet : public PlotData
{
public: 
    std::vector<std::pair<double, double>> &dataSet;
    PlotDataSet(std::vector<std::pair<double, double>> &DataSet, char Symbol = 'o', Style Style = Style::dots);
    std::tuple<double,double,double,double> getRange();
};
