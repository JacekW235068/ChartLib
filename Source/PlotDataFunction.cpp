#include "../Header/PlotDataFunction.hpp"

PlotDataFunction::PlotDataFunction(double (*Fun)(double),
     std::pair<double,double> XRange,
     double Accuracy,
     char Symbol,
     Color color, 
     Style Style) : PlotData(Symbol, Style, color),accuracy(Accuracy), fun(Fun) {
         min_x = XRange.first;
         max_x= XRange.second;
         double x = min_x;
         double y = fun(x);
         min_y = y;
         max_y = y;
         x+= accuracy;
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
void PlotDataFunction::setRange(std::pair<double,double> XRange){
     //TODO: optimize
     dataSet.clear();
     min_x = XRange.first;
     max_x= XRange.second;
     double x = min_x;
     double y = fun(x);
     min_y = y;
     max_y = y;
     x+= accuracy;
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
void PlotDataFunction::setFunction(double (*Fun)(double)){
     dataSet.clear();
     double x = min_x;
     double y = fun(x);
     min_y = y;
     max_y = y;
     x+= accuracy;
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
std::tuple<double,double,double,double> PlotDataFunction::getRange() const{
     return std::make_tuple(min_x,max_x,min_y,max_y);
}
const std::list<std::pair<double,double>>& PlotDataFunction::getData() const{
     return dataSet;
}
void PlotDataFunction::setAccuracy(double Accuracy){
     accuracy = Accuracy;
     dataSet.clear();
     double x = min_x;
     double y = fun(x);
     min_y = y;
     max_y = y;
     x+= accuracy;
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
double PlotDataFunction::getAccuracy(){
     return accuracy;
}