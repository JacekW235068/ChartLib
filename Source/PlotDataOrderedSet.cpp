#include <PlotDataOrderedSet.hpp>



namespace chart {
PlotDataOrderedSet::PlotDataOrderedSet(std::list<std::pair<double, double>> DataSet, char Symbol, std::string Name, Color color, Style Style, bool(*Compare)(const std::pair<double,double>&, const std::pair<double,double>&)) 
: PlotDataSet(DataSet, Symbol, Name, color, Style), 
compare(Compare){
    dataSet.sort(compare);
    getRange();
}

std::tuple<double,double,double,double> PlotDataOrderedSet::getRange() const{
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
    max_x = dataSet.back().first;
    for(const auto& data : dataSet){
        if (data.second < min_y)
            min_y = data.second;
        else if(data.second > max_y)
            max_y = data.second;
    }
    return std::make_tuple(min_x,max_x,min_y,max_y);
}

const std::list<std::pair<double,double>>& PlotDataOrderedSet::getData() const{
    return dataSet;
}

void PlotDataOrderedSet::setData(std::list<std::pair<double, double>> DataSet){
    dataSet = DataSet;
    dataSet.sort(compare);
    min_y = nan("");
    min_x = nan("");
    max_y = nan("");
    max_x = nan("");
}

void PlotDataOrderedSet::modifyDataSet(std::function<void(std::list<std::pair<double, double>>&)> lambda){
    lambda(dataSet);
    min_y = nan("");
    min_x = nan("");
    max_y = nan("");
    max_x = nan("");
}

}