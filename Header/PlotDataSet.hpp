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
    void setData(std::list<std::pair<double, double>> DataSet);
    void modifyDataSet(std::function<void(std::list<std::pair<double, double>>&)> lambda);
    PlotDataSet(std::list<std::pair<double, double>> DataSet = std::list<std::pair<double, double>>(), char Symbol = 'o', Color color = Color::non, Style Style = Style::dots);
    std::tuple<double,double,double,double> getRange() const;
    const std::list<std::pair<double,double>>& getData() const;
};  
