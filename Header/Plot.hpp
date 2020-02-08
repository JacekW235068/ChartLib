#pragma once

#include <utility>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <tuple>
#include <memory>

#include "./PlotData.hpp"
#include "../Enum/Scale.cpp"
#include "../Enum/Style.cpp"


class Plot
{
protected:
public:
    //DAT
    //chart settings
    //Style style;
    Scale scale;
    //terminal character cell aspect ratio X/Y
    double cellAspectRatio;
    //Array of printable data
    char** printableData;
    std::pair<unsigned, unsigned> windowSize;
    //symbol used for drawing chart
    //char symbol;
    std::list<std::reference_wrapper<PlotData>> dataSets;
    //range of chart
    double min_y;
    double min_x;
    double max_y;
    double max_x;
    double visible_min_y;
    double visible_min_x;
    double visible_max_y;
    double visible_max_x;

    //CONSTRUCTORS
    
    Plot(
        std::pair<unsigned, unsigned> WindowSize,
        Scale Scale = Scale::stretch,
        double CellAspectRatio = 0.5
        );
    ~Plot();

    //METHODS
    //find min/max X/Y
    void setRange();
    void createChart(std::pair<double,double> Xrange = {1,-1}, std::pair<double,double> Yrange = {1,-1});
    //void setRange(std::vector<std::pair<double, double>>& DataSet);
    //char** createChart(std::vector<std::pair<double, double>>& DataSet);
    //scaling methods, sets visible range
    void valueRange_scalex();
    void valueRange_scaley();
    void valueRange_stretch();
    //Style methods, "draws" symbol onto printable data
    void drawDots(PlotData& DataSet);
    void drawLines(PlotData& DataSet);
    //Draw line between p1 and p2
    void drawLine(std::pair<int,int> p1, std::pair<int,int> p2, char symbol);
    void addDataSet(PlotData& plot);

    
    //FRIENDS AND STUFF
    friend std::ostream& operator<<(std::ostream& s, const Plot& t);
};
