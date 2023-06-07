#pragma once
#include <ChartLib/Style.hpp>
#include <ChartLib/Scale.hpp>
#include <ChartLib/Color.hpp>

#include <list>
#include <tuple>
#include <cmath>
#include <string>

namespace chart{

class Plot;

class PlotData
{
protected:
    void setStyledSymbol();

    Color color;
    std::string name;
    char symbol;
    mutable double min_x;
    mutable double max_x;
    mutable double min_y;
    mutable double max_y;
    std::string styledSymbol;
public:
    Style style;
    virtual std::tuple<double,double,double,double> getRange() const = 0;
    virtual const std::list<std::pair<double,double>>& getData() const = 0;
    //TODO: Give it some config struct
    PlotData(char Symbol = 'o',std::string Name = "", Style Style = Style::Dots, Color color = Color::none);
    virtual ~PlotData();
    const std::string& getName() const;
    void setName(std::string Name);
    const char& getSymbol() const;
    void setSymbol(char Symbol);
    const Color& getColor() const;
    void setColor(Color Color);
    const std::string& getStyledSymbol() const;
};
 
}