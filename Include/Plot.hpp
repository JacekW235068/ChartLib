#pragma once
#include <PlotData.hpp>
#include <Scale.hpp>

#include <vector>
#include <list>
#include <iostream>
#include <tuple>
#include <map>



/**
 * ChartLib namespace
 *
 * Read more at https://github.com/JacekW235068/ChartLib/blob/master/README.md
 */
namespace chart {

/**
 * Main class implementing terminal chart.
 *
 * Contains datasets and is capable of printing them in given visibile range
 *
 */
class Plot
{
private:

    //DATA
    double cellAspectRatio;
    std::pair<unsigned, unsigned> windowSize;
    std::list<PlotData*> dataSets;
    //range of chart
    double visible_min_y;
    double visible_min_x;
    double visible_max_y;
    double visible_max_x;
    std::list<std::string> frame; //To bo removed
    //map with coords !(Y,X)! and symbols to draw onto chart 
    std::map<std::pair<int,int>, const std::string*> ChartMap;

    //METHODS
    //scaling methods, sets visible range
    void visibleRange_scalex(double center);
    void visibleRange_scaley(double center);
    void visibleRange_stretch();
    //"draws" symbols and saves them in map
    void drawDots(PlotData& DataSet);
    void drawLines(PlotData& plotData);
    //Draw line between p1 and p2
    void drawLine(std::pair<int, int> p1, std::pair<int,int> p2,const std::string &symbol);
    
public:

    //CONSTRUCTORS and stuff
    Plot(
        std::pair<unsigned, unsigned> WindowSize,
        double CellAspectRatio = 0.5
        );
    ~Plot();

    //METHODS



    //GET-SET
    //Imma let you guess what it does
    const std::pair<unsigned, unsigned>& getWindowSize() const;
    //Imma let you guess what it does
    const double& getCellAspectRatio() const;
    /**
     * @return Legend generated based on names and symbols of included datasets
     */
    std::string getLegend();
    /**
     * @return `[min_x,max_x,min_y,max_y]` min max values of all included datasets combined in format
     */
    std::tuple<double,double,double,double> getRange();
    /**
     * Sets visible range.
     *
     * @param scaling Enum with options for automatic scaling.
     * @param center With `AlignToX` option defines where Y center will be, defaults to avg.
     */
    void setVisibleRange(Scale scaling, double center = nan(""));
    /**
     * Sets visible range.
     *
     * @param Xrange Min-Max pair of X values
     * @param Yrange Min-Max pair of Y values
     */
    void setVisibleRange(std::pair<double,double> Xrange, std::pair<double,double> Yrange);

    void noFrame(); //To bo removed
    void simpleFrame(); //To bo removed
    void axisFrame(int Xprecission = 0, int Yprecission = 0); //To bo removed

    /**
     * Adds dataset to chart and prints it right away if visible range is set.
     *
     * @param plotData reference to single dataset.
     */
    void addDataSet(PlotData& plotData);
    /**
     * @overload
     * Adds datasets to chart and prints them right away if visible range is set.
     *
     * @param plotData `vector` of datasets references to include in chart
     */
    void addDataSet(std::vector<std::reference_wrapper<PlotData>>& plotData);
    /**
     * @overload
     * Adds datasets to chart and prints them right away if visible range is set.
     *
     * @param plotData `list` of datasets references to include in chart
     */
    void addDataSet(std::list<std::reference_wrapper<PlotData>>& plotData);
    /**
     * @overload
     * Adds datasets to chart and prints them right away if visible range is set.
     *
     * @param plotData `vector` of datasets pointers to include in chart
     */
    void addDataSet(std::vector<PlotData*>& plotData);
    /**
     * @overload
     * Adds datasets to chart and prints them right away if visible range is set.
     *
     * @param plotData `list` of datasets pointers to include in chart
     */
    void addDataSet(std::list<PlotData*>& plotData);
    /**
     * Removes a single dataset from chart
     *
     * @param removed reference to the **exact** dataset that is to be removed.
     */
    void removeDataSet(PlotData& removed);

    //FRIENDS AND STUFF
    friend std::ostream& operator<<(std::ostream& s, const Plot& t);
    friend PlotData::~PlotData();//please someone delete this line after I die, I don't want anyone seeing it.
};
}