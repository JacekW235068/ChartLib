#include "../Header/PlotDataSet.hpp"




PlotDataSet::PlotDataSet(std::list<std::pair<double, double>> DataSet, char Symbol, Color color, Style Style) 
: PlotData(Symbol, Style, color), 
dataSet(DataSet){
    getRange();
}

std::tuple<double,double,double,double> PlotDataSet::getRange() const{
    if(!std::isnan(min_x))
        return std::make_tuple(min_x,max_x,min_y,max_y);
    if(dataSet.empty()){
        min_y = nan("");
        min_x = nan("");
        max_y = nan("");
        max_x = nan("");
        return std::make_tuple(min_x,max_x,min_y,max_y);
    }
    auto& element = *dataSet.begin();
    min_y = element.second;
    min_x = element.first;
    max_y = element.second;
    max_x = element.first;
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

const std::list<std::pair<double,double>>& PlotDataSet::getData() const{
    return dataSet;
}
/* std::list<std::pair<double,double>> PlotDataSet::getData(std::pair<double,double> Xrange, std::pair<double,double> Yrange) const{
    std::list<std::pair<double,double>> visibleData;
    for(const auto& data : dataSet){
        if(data.first >= Xrange.first && data.first <= Xrange.second
        && data.second >= Yrange.first && data.second <= Yrange.second)
            visibleData.push_back(data);
    }
    return visibleData;}*/

void PlotDataSet::setData(std::list<std::pair<double, double>> DataSet){
    dataSet = DataSet;
    min_y = nan("");
    min_x = nan("");
    max_y = nan("");
    max_x = nan("");
}

void PlotDataSet::modifyDataSet(std::function<void(std::list<std::pair<double, double>>&)> lambda){
    lambda(dataSet);
    min_y = nan("");
    min_x = nan("");
    max_y = nan("");
    max_x = nan("");
}

