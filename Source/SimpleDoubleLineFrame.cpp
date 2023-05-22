#include <ChartLib/SimpleDoubleLineFrame.hpp>

namespace chart{


std::tuple<int,int,int,int> SimpleDoubleLineFrame::drawFrame(
        std::map<std::pair<int,int>, std::string>& ChartMap,
        const std::pair<uint16_t,uint16_t>& WindowSize,
        const std::tuple<double,double,double,double>& VisibleRange) {
    // Frame corners
    ChartMap[{-1,-1}] = u8"╔";
    ChartMap[{WindowSize.second,-1}] = u8"╚";
    ChartMap[{-1,WindowSize.first}] = u8"╗";
    ChartMap[{WindowSize.second,WindowSize.first}] = u8"╝";
    // Top and bottom
    for (int i=0; i < WindowSize.first; i++){
        ChartMap[{-1,i}] = u8"═";
        ChartMap[{WindowSize.second,i}] = u8"═";
    }
    // Left and right
    for(int i=0; i < WindowSize.second; i++){
        ChartMap[{i,-1}] = u8"║";
        ChartMap[{i,WindowSize.first}] = u8"║";
    }
    return {-1,WindowSize.first,-1,WindowSize.second};
}

bool SimpleDoubleLineFrame::isForced() const {
    return true;
}
}