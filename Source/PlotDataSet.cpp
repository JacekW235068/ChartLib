#include "../Header/PlotDataSet.hpp"




PlotDataSet::PlotDataSet(std::list<std::pair<double, double>> DataSet, char Symbol, Style Style) 
: PlotData(Symbol, Style), 
dataSet(DataSet){

}

std::tuple<double,double,double,double> PlotDataSet::getRange() const{
    if(dataSet.empty()){
        return std::make_tuple(nan(""),nan(""),nan(""),nan(""));
    }
    auto& element = *dataSet.begin();
    double min_y = element.second;
    double min_x = element.first;
    double max_y = element.second;
    double max_x = element.first;
    for(const auto& data : dataSet){
        if (data.first < min_x)
            min_x = data.first;
        else if(data.first > max_x)
            max_x = data.first;
        if (data.second < min_y)
            min_y = data.second;
        else if(data.second > max_y)
            max_y = data.second;
    }
    return std::make_tuple(min_x,max_x,min_y,max_y);
}

std::list<std::pair<double,double>>& PlotDataSet::getData(){
    return dataSet;
}
/* std::list<std::pair<double,double>> PlotDataSet::getData(std::pair<double,double> Xrange, std::pair<double,double> Yrange) const{
    std::list<std::pair<double,double>> visibleData;
    for(const auto& data : dataSet){
        if(data.first >= Xrange.first && data.first <= Xrange.second
        && data.second >= Yrange.first && data.second <= Yrange.second)
            visibleData.push_back(data);
    }
    return visibleData;
} */
