#pragma once

#include <Style.hpp>
#include <Scale.hpp>
#include <Color.hpp>
#include <list>
#include <tuple>
#include <cmath>
#include <string>

namespace chart{

class Plot;

class PlotData
{

protected:
    //cached range
    mutable double min_x;
    mutable double min_y;
    mutable double max_x;
    mutable double max_y;
    //Symbol to represent data on chart
    Color color;
    std::string name;
    std::string styledSymbol;
    static std::string mapColor(Color color);
    void setStyledSymbol();
    std::list<Plot*> plots;
    void dataSetModified();
public:
    char symbol;
    //line,dots, that kinda stuff
    Style style;
    virtual std::tuple<double,double,double,double> getRange() const = 0;
    virtual const std::list<std::pair<double,double>>& getData() const = 0;
    PlotData(char Symbol = 'o',std::string Name = "", Style Style = Style::dots, Color color = Color::none);
    virtual ~PlotData();
    const std::string& getName() const;
    void setName(std::string Name);
    const char& getSymbol() const;
    void setSymbol(char Symbol);
    const Color& getColor() const;
    void setColor(Color Color);
    const std::string& getStyledSymbol() const;
    friend class Plot;
};
 
}