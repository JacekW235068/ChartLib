#include "../Header/PlotDataSet.hpp"




PlotDataSet::PlotDataSet(std::vector<std::pair<double, double>> &DataSet, char Symbol, Style Style) 
: PlotData(Symbol, Style), 
dataSet(DataSet){

}

std::tuple<double,double,double,double> PlotDataSet::getRange(){
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
