#pragma once
#include "../Enum/Style.cpp"
#include <tuple>
#include <cmath>
#include <functional>
#include "./PlotDataSet.hpp"

class PlotDataOrderedSet : public PlotDataSet
{
private:
    bool(*compare)(const std::pair<double,double>&, const std::pair<double,double>&);
public:
    void setData(std::list<std::pair<double, double>> DataSet);
    void modifyDataSet(std::function<void(std::list<std::pair<double, double>>&)> lambda);
    PlotDataOrderedSet(std::list<std::pair<double, double>> DataSet = std::list<std::pair<double, double>>(),
     char Symbol = 'o',
     Color color = Color::non, 
     Style Style = Style::dots,
     bool(*Compare)(const std::pair<double,double>&, const std::pair<double,double>&) = [](const std::pair<double,double> &a, const std::pair<double,double> &b){return a < b;});
    std::tuple<double,double,double,double> getRange() const;
    const std::list<std::pair<double,double>>& getData() const;
};  
