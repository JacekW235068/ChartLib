#pragma once
#include "../Enum/Style.cpp"
#include "../Enum/Scale.cpp"
#include "../Enum/Color.cpp"
#include <list>
#include <tuple>
#include <cmath>
#include <string>

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
    
    std::string styledSymbol;
    static std::string mapColor(Color color);
    void setStyledSymbol();
public:
    char symbol;
    //line,dots, that kinda stuff
    Style style;
    virtual std::tuple<double,double,double,double> getRange() const = 0;
    virtual const std::list<std::pair<double,double>>& getData() const = 0;
    PlotData(char Symbol = 'o', Style Style = Style::dots, Color color = Color::non);
    PlotData(const PlotData& plot) = delete; //not sure if it does anything but just keeping it as temporary check for copying
    virtual ~PlotData();
    char getSymbol();
    void setSymbol(char Symbol);
    Color getColor();
    void setColor(Color Color);
    const std::string& getStyledSymbol();
};
 
