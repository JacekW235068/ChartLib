#pragma once

#include <utility>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <tuple>
#include <memory>
#include <map>
#include <sstream>

#include "./PlotData.hpp"
#include "../Enum/Scale.cpp"
#include "../Enum/Style.cpp"


class Plot
{
private:
    //DATA
    //Style style;
    Scale scale;
    double cellAspectRatio;
    //Array of printable data
    std::string printableChart;
    std::pair<unsigned, unsigned> windowSize;
    //symbol used for drawing chart
    //char symbol;
    std::list<std::reference_wrapper<PlotData>> dataSets;
    //range of chart
    double visible_min_y;
    double visible_min_x;
    double visible_max_y;
    double visible_max_x;

    

    //METHODS
    //scaling methods, sets visible range
    void valueRange_scalex(double center);
    void valueRange_scaley(double center);
    void valueRange_stretch();
    //Style methods, "draws" symbol onto printable data
    void drawDots(PlotData& DataSet, std::map<std::pair<double,double>,  const std::string*>& chartMap);
    void drawLines(PlotData& plotData, std::map<std::pair<double,double>,  const std::string*>& chartMap);
    //Draw line between p1 and p2
    void drawLine(std::pair<int, int> p1, std::pair<int,int> p2,const std::string &symbol, std::map<std::pair<double,double>,  const std::string*>& chartMap);
    
    std::tuple<double,double,double,double> getRange();
    //DataModification
public:

    //OPTIONS MODIFICATION
    //Requires Recreating chart!!!
    void setWindowSize(std::pair<unsigned, unsigned> WindowSize);
    //Requires Recreating chart!!!
    void setCellAspectratio(double CellAspectRatio);
    //Requires Recreating chart!!!
    void setScaling(Scale Scale);
    //ATTENCTION: SKETCHY AS FUCK
    void RemoveData(PlotData& removed);
    //OPTIONS ACCESS
    std::pair<unsigned, unsigned> getWindowSize();
    double getCellAspectRation();
    Scale getScaling();

    //CONSTRUCTORS and stuff
    Plot(
        std::pair<unsigned, unsigned> WindowSize,
        Scale Scale = Scale::stretch,
        double CellAspectRatio = 0.5
        );
    ~Plot();

    //METHODS
    void addSimpleFrame();
    void clearChart();
    void createChart(double center = nan(""));
    void createChart(std::pair<double,double> Xrange, std::pair<double,double> Yrange);
    void addDataSet(PlotData& plot);

    //FRIENDS AND STUFF
    friend std::ostream& operator<<(std::ostream& s, const Plot& t);
};
