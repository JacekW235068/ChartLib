#pragma once
#include <tuple>
#include <string>
#include <map>


namespace chart
{
/**
 * Decoration interface providing user with functionality implementing and drawing on generated plot.
 */
class IDecoration{
public:
    /**
     * Prints any sort of additional characters onto finished plot.
     * After IFrame class is assigned to a plot object it is called everytime plot is redrawn.
     *
     * @param ChartMap Map of !(Y,X)! coords with printed plot
     * @param WindowSize Dimmensions of a window described by `ChartMap`
     * @param VisibleRange Visible range of drawn plot
     * 
     * @return min max X and min max Y of ChartMap after changes made by decoration (can also be negative values if something is drawn to the left of above chart)
     */
    virtual std::tuple<int,int,int,int> drawFrame(
        std::map<std::pair<int,int>, std::string>& ChartMap,
        const std::pair<uint16_t,uint16_t>& WindowSize,
        const std::tuple<double,double,double,double>& VisibleRange) const = 0;
    
    virtual bool isForced() const = 0;
};
}