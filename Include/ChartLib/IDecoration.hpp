#pragma once
#include <cstdint>
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
    //TODO: cleanup return type
    /**
     * Prints any sort of additional characters onto finished plot.
     * After IFrame class is assigned to a plot object it is called everytime plot is redrawn.
     *
     * @param ChartMap Map of !(Y,X)! coords with printed plot
     * @param WindowSize Dimmensions of a window described by `ChartMap`
     * @param VisibleRange Visible range of drawn plot
     */
    virtual void drawFrame(
        std::map<std::pair<int,int>, std::string>& ChartMap,
        const std::pair<uint16_t,uint16_t>& WindowSize,
        const std::tuple<double,double,double,double>& VisibleRange) = 0;
    
    //TODO: give minigful name and change to some kind of property
    virtual bool isForced() const = 0;
    virtual ~IDecoration() = default;
};
}