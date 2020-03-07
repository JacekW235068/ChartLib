#pragma once
#include "../Enum/Style.cpp"
#include <vector>
#include <tuple>
#include <cmath>
#include <functional>

#include "./PlotData.hpp"

class PlotDataSet : public PlotData
{
protected:
    std::list<std::pair<double, double>> dataSet;
public:
    virtual void setData(std::list<std::pair<double, double>> DataSet);
    virtual void modifyDataSet(std::function<void(std::list<std::pair<double, double>>&)> lambda);
    PlotDataSet(std::list<std::pair<double, double>> DataSet = std::list<std::pair<double, double>>(), char Symbol = 'o', Color color = Color::none, Style Style = Style::dots);
    virtual std::tuple<double,double,double,double> getRange() const;
    virtual const std::list<std::pair<double,double>>& getData() const;
    virtual ~PlotDataSet();

};  
