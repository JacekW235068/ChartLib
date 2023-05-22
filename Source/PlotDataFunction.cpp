#include <ChartLib/PlotDataFunction.hpp>



namespace chart {

PlotDataFunction::PlotDataFunction(double (*Fun)(double),
     std::pair<double,double> XRange,
     double Accuracy,
     char Symbol,
     std::string Name,
     Color color, 
     Style Style) 
     : PlotData(Symbol,Name , Style, color),accuracy(Accuracy),
     fun(Fun) {
          setRange(XRange);
     }
void PlotDataFunction::setRange(std::pair<double,double> XRange){
     min_x = XRange.first;
     max_x = XRange.second;
     regenerateDataSet();
}

void PlotDataFunction::setFunction(double (*Fun)(double)){
     fun = Fun;
     regenerateDataSet();
}
std::tuple<double,double,double,double> PlotDataFunction::getRange() const{
     return std::make_tuple(min_x,max_x,min_y,max_y);
}
const std::list<std::pair<double,double>>& PlotDataFunction::getData() const{
     return dataSet;
}
void PlotDataFunction::setAccuracy(double Accuracy){
     accuracy = Accuracy;
     regenerateDataSet();
}
double PlotDataFunction::getAccuracy(){
     return accuracy;
}

void PlotDataFunction::regenerateDataSet(){
     dataSet.clear();
     double x = min_x;
     double y = fun(x);
     min_y = y;
     max_y = y;
     for(x ; x <= max_x; x= x+accuracy){
          y = fun(x);
          dataSet.push_back({x,y});
          if(y < min_y)
               min_y = y;
          else if(y > max_y){
               max_y = y;
          }
     }
}

}