#pragma once
#include <ChartLib/PlotData.hpp>
#include <ChartLib/Scale.hpp>
#include <ChartLib/IDecoration.hpp>

#include <vector>
#include <list>
#include <iostream>
#include <tuple>
#include <map>
#include <memory>


namespace chart {

/**
 * Main class implementing terminal chart.
 *
 * Contains datasets references and is capable of printing them in given visibile range
 *
 */
class Plot
// TODO: Naming, chart - plot ?
{
private:

    // DATA
    double cellAspectRatio;
    // TODO: unify XY vs YX
    std::pair<uint16_t, uint16_t> windowSize;
    // TODO: Make it set?
    std::list<std::weak_ptr<PlotData>> dataSets;
    std::list<IDecoration*> decorations;
    // range of chart
    double visible_min_y;
    double visible_min_x;
    double visible_max_y;
    double visible_max_x;
    // map with coords !(Y,X)! and symbols to draw onto chart 
    std::map<std::pair<int,int>, std::string> ChartMap;

    // METHODS
    // scaling methods, sets visible range
    void visibleRange_scalex(double center);
    void visibleRange_scaley(double center);
    void visibleRange_stretch();
    // "draws" symbols and saves them in map
    void drawDots(PlotData& DataSet);
    void drawLines(PlotData& plotData);
    // Draw line between p1 and p2
    void drawLine(std::pair<long,long> p1, std::pair<long,long> p2,const std::string &symbol);
    // PlotDataConnection methods
    void drawOnChartMap(PlotData& plotData);
    /**
     * Generates a plot and its decorations in [y,x]->char map form
     */
    void generate();
    /**
     * Finds draing borders after generating decorations
     * 
     * @note plot starts at 0,0 therefore decorations can generate values below 0 
     * 
     * @return min_x,max_x,min_y,max_y
     */
    std::tuple<int,int,int,int> getAdjustedBoundries();
public:

    // CONSTRUCTORS
    Plot(
        std::pair<uint16_t, uint16_t> WindowSize,
        double CellAspectRatio = 0.5
    );

    // METHODS
    /**
     * @return returns XY window size
     */
    const std::pair<uint16_t, uint16_t>& getWindowSize() const;
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

    /**
     * Adds dataset to chart and prints it right away if visible range is set.
     *
     * @param plotData pointer to single dataset.
     */
    void addDataSet(const std::weak_ptr<PlotData> plotData);
    /**
     * Removes a single dataset from chart
     *
     * @param removed reference to the **exact** dataset that is to be removed.
     */
    void removeDataSet(std::weak_ptr<PlotData> removed);

    void addDecoration(IDecoration* decoration);
    void addDecoration(std::vector<IDecoration*> decorations);
    void addDecoration(std::list<IDecoration*> decorations);
    void removeDecoration(IDecoration* decoration);

    /**
     * Print created chart in string format
     * 
     * @return Chart in strgin format
     */
    std::string print();
};
}
