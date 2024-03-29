#include <ChartLib/PlotDataSet.hpp>


namespace chart {
// TODO: Do something about sending data to constructor
PlotDataSet::PlotDataSet(std::list<std::pair<double, double>> DataSet, char Symbol, std::string Name, Color color, Style Style) 
: PlotData(Symbol, Name, Style, color), 
dataSet(DataSet){
    getRange();
}
PlotDataSet::~PlotDataSet(){

}

// TODO: remove caching and calculate on the spot
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

void PlotDataSet::setData(std::list<std::pair<double, double>>& DataSet){
    dataSet = DataSet;
    min_y = nan("");
    min_x = nan("");
    max_y = nan("");
    max_x = nan("");
}
}