#include <ChartLib/YAxisLabels.hpp>

#include <iomanip>
#include <cmath>
#include <iostream>

namespace chart{

YAxisLabels::YAxisLabels(uint Precision, double Jump, double Start) : precision(Precision), jump(Jump), start(Start)
{}

// TODO: Rewrite after tests
void YAxisLabels::drawFrame(
        std::map<std::pair<int,int>, std::string>& ChartMap,
        const std::pair<uint16_t,uint16_t>& WindowSize,
        const std::tuple<double,double,double,double>& VisibleRange) {
    using namespace std;

    double visibleRangeY = get<3>(VisibleRange) - get<2>(VisibleRange);
    if(jump <= 0.0) 
        jump = pow(10.0,-precision);
    double y = start;
    while (y > get<2>(VisibleRange)){
        y -= jump;
    }
    while (y < get<2>(VisibleRange)){
        y += jump;
    }
    int yCell = static_cast<int>(round((get<3>(VisibleRange) - y)/visibleRangeY*(WindowSize.second-1)));
    stringstream stream;
    stream << fixed << setprecision(precision);
    stream << y;
    string s = stream.str();
    while (yCell >= 0){
        stream.str(string());
        stream.clear();
        stream << (y==-0.0?0.0:y); // workaround for '-0.0'
        string s = stream.str();
        for(size_t i = 0; i < s.length(); i++){
            ChartMap[{yCell,WindowSize.first+1+i}] = s[i];
        }
        ChartMap[{yCell,WindowSize.first}] = '.';
        y += jump;
        yCell = static_cast<int>(round((get<3>(VisibleRange) - y)/visibleRangeY*(WindowSize.second-1)));
    }
}

bool YAxisLabels::isForced() const {
    return true;
}

void YAxisLabels::setPrecision(int Precision){
    precision = Precision; 
}
int YAxisLabels::getPrecision(){
    return precision;
}

void YAxisLabels::setJump(double Jump){
    jump = Jump;
}
double YAxisLabels::getJump(){
return jump;
}

void YAxisLabels::setStar(double Start){
    start = Start;
}
double YAxisLabels::getStar(){
    return start;
}
}