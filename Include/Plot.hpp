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

#include <PlotData.hpp>
#include <Scale.hpp>
#include <Style.hpp>

namespace chart {
class Plot
{
private:
    //DATA
    //Style style;
    Scale scale;
    double cellAspectRatio;
    std::pair<unsigned, unsigned> windowSize;
    //symbol used for drawing chart
    //char symbol;
    std::list<std::reference_wrapper<PlotData>> dataSets;
    //range of chart
    double visible_min_y;
    double visible_min_x;
    double visible_max_y;
    double visible_max_x;
    std::list<std::string> frame;
    //map with coords !(Y,X)! and symbols to draw onto chart 
    std::map<std::pair<int,int>, const std::string*> ChartMap;

    //METHODS
    //scaling methods, sets visible range
    void valueRange_scalex(double center);
    void valueRange_scaley(double center);
    void valueRange_stretch();
    //Style methods, "draws" symbol onto printable data
    void drawDots(PlotData& DataSet);
    void drawLines(PlotData& plotData);
    //Draw line between p1 and p2
    void drawLine(std::pair<int, int> p1, std::pair<int,int> p2,const std::string &symbol);
    
    std::tuple<double,double,double,double> getRange();
public:
    void setValueRange(Scale scaling, double center = nan(""));
    void setValueRange(std::pair<double,double> Xrange, std::pair<double,double> Yrange);
    //OPTIONS MODIFICATION
    //Requires Recreating chart!!!
    void setWindowSize(std::pair<unsigned, unsigned> WindowSize);
    //Requires Recreating chart!!!
    void setCellAspectratio(double CellAspectRatio);
    //Requires Recreating chart!!!
    void setScaling(Scale Scale);
    //ATTENCTION: SKETCHY AS FUCK
    void removeDataSet(PlotData& removed);
    //OPTIONS ACCESS
    const std::pair<unsigned, unsigned>& getWindowSize() const;
    const double& getCellAspectRation() const;
    const Scale& getScaling() const;
    //CONSTRUCTORS and stuff
    Plot(
        std::pair<unsigned, unsigned> WindowSize,
        Scale Scale = Scale::stretch,
        double CellAspectRatio = 0.5
        );
    ~Plot();

    //METHODS
    std::string getLegend();
    void zeroPointAxis();
    //Draws straight line parallel to x axis
    void xLine(double y);
    //Draws straight line parallel to y axis
    void yLine(double x);
    void minmaxY(PlotData& data);
    void minmaxX(PlotData& data);
    void noFrame();
    void simpleFrame();
    void axisFrame(int Xprecission = 0, int Yprecission = 0);
    void clearChart();
    void createChart();

    void addDataSet(PlotData& plot);
    void addDataSets(std::vector<std::reference_wrapper<PlotData>> plotData);
    void addDataSets(std::list<std::reference_wrapper<PlotData>> plotData);
    void addDataSets(std::vector<PlotData*> plotData);
    void addDataSets(std::list<PlotData*> plotData);
    //FRIENDS AND STUFF
    friend std::ostream& operator<<(std::ostream& s, const Plot& t);
    friend PlotData::~PlotData();
};
}