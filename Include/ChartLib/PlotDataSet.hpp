#pragma once
#include <ChartLib/Style.hpp>
#include <ChartLib/PlotData.hpp>

#include <vector>
#include <tuple>
#include <cmath>
#include <functional>

namespace chart {

class PlotDataSet : public PlotData
{
protected:
    std::list<std::pair<double, double>> dataSet;
public:
    virtual void setData(std::list<std::pair<double, double>>& DataSet);
    PlotDataSet(std::list<std::pair<double, double>> DataSet = std::list<std::pair<double, double>>(), char Symbol = 'o',std::string Name = "", Color color = Color::none, Style Style = Style::dots);
    virtual std::tuple<double,double,double,double> getRange() const;
    virtual const std::list<std::pair<double,double>>& getData() const;
    virtual ~PlotDataSet();

};  
}