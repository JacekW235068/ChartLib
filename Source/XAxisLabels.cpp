#include <ChartLib/XAxisLabels.hpp>

#include <iomanip>
#include <cmath>
#include <iostream>

namespace chart{

XAxisLabels::XAxisLabels(uint Precision, double Jump, double Start) : precision(Precision), jump(Jump), start(Start)
{}

void XAxisLabels::drawFrame(
        std::map<std::pair<int,int>, std::string>& ChartMap,
        const std::pair<uint16_t,uint16_t>& WindowSize,
        const std::tuple<double,double,double,double>& VisibleRange) {
    using namespace std;

    double visibleRangeX = get<1>(VisibleRange) - get<0>(VisibleRange);
    if(jump <= 0.0) 
        jump = pow(10.0,-precision);
    double x = start;
    while (x > get<0>(VisibleRange)){
        x -= jump;
    }
    while (x < get<0>(VisibleRange)){
        x += jump;
    }
    int xCell = static_cast<int>(round((x-get<0>(VisibleRange))/visibleRangeX*(WindowSize.first-1)));
    int lastNumberCell = -21;
    int firstNumberCell;
    stringstream stream;
    stream << fixed << setprecision(precision);
    stream << x;
    string s = stream.str();

    while (xCell < WindowSize.first){
        stream.str(string());
        stream.clear();
        stream << (x==-0.0?0.0:x); // workaround for '-0.0'
        string s = stream.str();
        firstNumberCell = xCell - s.length()/2;
        if (lastNumberCell < firstNumberCell){
            for(size_t i = 0; i < s.length(); i++){
                ChartMap[{WindowSize.second+1,firstNumberCell++}] = s[i];
            }
            lastNumberCell = firstNumberCell;
            ChartMap[{WindowSize.second,xCell}] = '.';
        }
        x += jump;
        xCell = static_cast<int>(round((x-get<0>(VisibleRange))/visibleRangeX*(WindowSize.first-1)));
    }
}

bool XAxisLabels::isForced() const {
    return true;
}

void XAxisLabels::setPrecision(int Precision){
    precision = Precision; 
}
int XAxisLabels::getPrecision(){
    return precision;
}

void XAxisLabels::setJump(double Jump){
    jump = Jump;
}
double XAxisLabels::getJump(){
return jump;
}

void XAxisLabels::setStar(double Start){
    start = Start;
}
double XAxisLabels::getStar(){
    return start;
}
}