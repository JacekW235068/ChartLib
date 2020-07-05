#pragma once
#include <Style.hpp>
#include <tuple>
#include <cmath>
#include <functional>
#include <PlotData.hpp>

class PlotDataFunction : public PlotData
{
protected:
    std::list<std::pair<double, double>> dataSet;
    double accuracy;
    double (*fun)(double) ;
public:
    PlotDataFunction(double (*Fun)(double),
     std::pair<double,double> XRange,double Accuracy,
     char Symbol = 'o',
     std::string Name = "",
     Color color = Color::none, 
     Style Style = Style::dots);
    void setRange(std::pair<double,double> XRange);
    void setFunction(double (*Fun)(double));
    std::tuple<double,double,double,double> getRange() const;
    const std::list<std::pair<double,double>>& getData() const;
    double getAccuracy();
    void setAccuracy(double Accuracy);
};  
