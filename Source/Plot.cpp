 #include "../Header/Plot.hpp"




Plot::Plot(std::pair<unsigned, unsigned> WindowSize,
    Scale Scale,
    double CellAspectRatio
    ) :
    windowSize(WindowSize),
    scale(Scale),
    cellAspectRatio(CellAspectRatio)
{
    dataSets = std::list<std::reference_wrapper<PlotData>>();
}
Plot::~Plot()
{
}
void Plot::addDataSet(PlotData& plot){
    dataSets.push_back(plot);
}


void Plot::createChart(double center){
    //calculate visible value range, set limits
    static const std::string endLine= "\n";
    getRange();
    switch (scale)
    {
    case Scale::AlignToX:
        valueRange_scalex(center);
        break;
    case Scale::AlignToY:
        valueRange_scaley(center);
        break;    
    default:
        valueRange_stretch();
        break;
    }
    //map with coords !(Y,X)! and symbols to draw onto chart 
    std::map<std::pair<double,double>, const std::string*> ChartMap;
    //prefill endlines
    for(int i = 0; i< windowSize.second; i++){
        ChartMap[{i,windowSize.first}] = &endLine;
    }
    //draw chart
    for (PlotData& dataSet : dataSets)
    switch (dataSet.style)
    {
    case Style::Linear:
        drawLines(dataSet, ChartMap);
        break;
    default:
        drawDots(dataSet, ChartMap);
        break;
    }
    printableChart.clear();
    int x = -1;
    for(const auto& [coords,symbol] : ChartMap){
        //fill spaces
        printableChart += std::string(coords.second-x-1, ' ');
        //place symbol
        printableChart += *symbol;
        if(*symbol == "\n")
            x=-1;
        else
            x = coords.second;
    }

}

void Plot::createChart(std::pair<double,double> Xrange, std::pair<double,double> Yrange){
    static const std::string endLine= "\n";
    //calculate visible value range, set limits
    if (Xrange.first >= Xrange.second || Yrange.first >= Yrange.second)
        return; //should probably throw here
    visible_min_x = Xrange.first;
    visible_max_x = Xrange.second;
    visible_min_y = Yrange.first;
    visible_max_y = Yrange.second;
    //map with coords !(Y,X)! and symbols to draw onto chart 
    std::map<std::pair<double,double>, const std::string*> ChartMap;
    //prefill endlines
    for(int i = 0; i< windowSize.second; i++){
        ChartMap[{i,windowSize.first}] = &endLine;
    }
    //draw chart
    for (PlotData& dataSet : dataSets)
    switch (dataSet.style)
    {
    case Style::Linear:
        drawLines(dataSet, ChartMap);
        break;
    default:
        drawDots(dataSet, ChartMap);
        break;
    }
    printableChart.clear();
    int x = -1;
    for(const auto& [coords,symbol] : ChartMap){
        //fill spaces
        printableChart += std::string(coords.second-x-1, ' ');
        //place symbol
        printableChart += *symbol;
        if(*symbol == "\n")
            x=-1;
        else
            x = coords.second;
    }
}

void Plot::clearChart(){
    printableChart.clear();
}


void Plot::valueRange_stretch(){
    auto [min_x,max_x,min_y,max_y] = getRange();
    //set limits
    visible_min_y = min_y;
    visible_min_x = min_x;
    visible_max_y = max_y;
    visible_max_x = max_x;
    // single point/straight line scenario
    if(min_y == max_y){
        visible_max_y += 0.5;
        visible_min_y -= 0.5;
    }
    if(min_x == max_x){
        visible_max_x += 0.5;
        visible_min_x -= 0.5;
    }
}

void Plot::valueRange_scaley(double center){
    auto [min_x,max_x,min_y,max_y] = getRange();
    if (std::isnan(center))
        center = (max_x+min_x)/2;
    //range of y axis
    double valueRangeY = abs(max_y - min_y);
    visible_min_y = min_y;
    visible_max_y = max_y;
    // single point/straight line scenario
    if(valueRangeY == 0){
        visible_min_y -= 0.5;
        visible_max_y += 0.5;
        valueRangeY = 1.0;
    }
    //y range per cell (with cell aspect ratio included) * X cells
    double valueRangeX = valueRangeY/(windowSize.second)*windowSize.first*cellAspectRatio;
    visible_min_x = center - valueRangeX/2;
    visible_max_x = center + valueRangeX/2;
}


void Plot::valueRange_scalex(double center){
    auto [min_x,max_x,min_y,max_y] = getRange();
    if (std::isnan(center))
        center = (max_y+min_y)/2;
    //range of x axis
    double valueRangeX = abs(max_x - min_x);
    visible_min_x = min_x;
    visible_max_x = max_x;
    // single point/straight line scenario
    if(valueRangeX == 0){
        visible_min_x -= 0.5;
        visible_max_x += 0.5;
        valueRangeX = 1.0;
    }
    //x range per cell  * y cells (with cell aspect ratio included)
    double valueRangeY = valueRangeX/(windowSize.first*cellAspectRatio)*windowSize.second;
    //set limits
    visible_min_y = center - valueRangeY/2;
    visible_max_y = center + valueRangeY/2;
}

void Plot::drawDots(PlotData& DataSet, std::map<std::pair<double,double>,  const std::string*>& chartMap){
    double visibleRangeX = visible_max_x - visible_min_x;
    double visibleRangeY = visible_max_y - visible_min_y;
    
    //asign each element to place in chart
    std::list< std::pair<double, double> > listElements = DataSet.getData();

    for(const auto& data : DataSet.getData()){
       if(data.first >= visible_min_x && data.first <= visible_max_x &&
	  data.second >= visible_min_x && data.second <= visible_max_x ){
	int y = static_cast<int>(round((data.second-visible_min_y)/visibleRangeY*(windowSize.second-1)));
        int x =static_cast<int>(round((data.first-visible_min_x)/visibleRangeX*(windowSize.first-1)));
        //check if in visible range
       	if(x >= 0 && y >= 0 && x < windowSize.first && y < windowSize.second)
       		   chartMap[{windowSize.second-y-1, x}] = &DataSet.getStyledSymbol();
  	 }
    }
}

void Plot::drawLines(PlotData& plotData, std::map<std::pair<double,double>,  const std::string*>& chartMap){
    double visibleRangeX = visible_max_x - visible_min_x;
    double visibleRangeY = visible_max_y - visible_min_y;
    auto& dataSet = plotData.getData();
    //coords for first point
    std::pair<int,int> previousCoords(
        static_cast<int>(round((dataSet.begin()->first-visible_min_x)/visibleRangeX*(windowSize.first-1))),
        static_cast<int>(round((dataSet.begin()->second-visible_min_y)/visibleRangeY*(windowSize.second-1)))
    );  
    for(const std::pair<double,double>& data : dataSet){
        int y = static_cast<int>(round((data.second-visible_min_y)/visibleRangeY*(windowSize.second-1)));
        int x =static_cast<int>(round((data.first-visible_min_x)/visibleRangeX*(windowSize.first-1)));
        drawLine(previousCoords, {x,y}, plotData.getStyledSymbol(),chartMap);
        //new point becomes old
        previousCoords = {x,y};
    }
}


void Plot::drawLine(std::pair<int, int> p1, std::pair<int,int> p2,const std::string &symbol, std::map<std::pair<double,double>,  const std::string*>& chartMap){
    //straight line X
    if (p1.first == p2.first){
        //is line in visible field?
        if (p1.first >= 0 && p1.first < windowSize.first){
            //draw line from low limit to high limit (limits include max window height)
            int YlowerCoords = std::max(0, std::min(p1.second, p2.second));
            int YupperCoords = std::min(static_cast<int>(windowSize.second-1), std::max(p1.second, p2.second));
            while (YlowerCoords <= YupperCoords){
       		    chartMap[{windowSize.second-YlowerCoords-1, p1.first}] = &symbol;
                YlowerCoords ++;
            } 
        }
    }
    //Same but on y
    else if (p1.second == p2.second){
        if (p1.second >= 0 && p1.second < windowSize.second){
            int XlowerCoords = std::max(0, std::min(p1.first, p2.first));
            int XupperCoords = std::min(static_cast<int>(windowSize.first-1), std::max(p1.first, p2.first));
            while (XlowerCoords <= XupperCoords){
       		    chartMap[{windowSize.second -p1.second -1, XlowerCoords}] = &symbol; 
                XlowerCoords ++;
            }
        } 
    }
    else{
        unsigned x_length = abs(p1.first - p2.first);
        unsigned y_length = abs(p1.second - p2.second);
        //pick one with higher 'resolution' so it fills everything between two points
        if(x_length > y_length){
            //y=ax+b
            double a = static_cast<double>(p2.second - p1.second) / (p2.first - p1.first);
            double b = static_cast<double>(p1.second) - (a*p1.first);
            //set limit for drawed line
            double XlowerLimit;
            double XupperLimit;
            if(a > 0){
                XlowerLimit = -b/a;
                XupperLimit = (windowSize.second -1 - b)/a;
            }else{
                XlowerLimit = (windowSize.second -1 - b)/a;
                XupperLimit = -b/a;
            }
            //compare calculated limits with visible window limits and point start,end coords
            int XlowerCoord = std::max(std::max( 0, static_cast<int>(round(XlowerLimit))), std::min(p1.first, p2.first));
            int XupperCoord = std::min( std::min(static_cast<int>(windowSize.first -1), static_cast<int>(round(XupperLimit))), std::max(p1.first, p2.first));
            //y calculated as function of x
            double y = a*XlowerCoord + b;
            while(XlowerCoord <= XupperCoord){
       		        chartMap[{windowSize.second- static_cast<int>(round(y))-1, XlowerCoord++}] = &symbol; 
                y+=a;
            }
        }else{
            //same as above exept with this formula
            //x = ay + b
            double a = static_cast<double>(p2.first - p1.first) / (p2.second - p1.second);
            double b = static_cast<double>(p1.first) - (a*p1.second);
            double YlowerLimit;
            double YupperLimit;
            if(a > 0){
                YlowerLimit = -b/a;
                YupperLimit = (windowSize.first -1 - b)/a;
            }else{
                YlowerLimit = (windowSize.first -1 - b)/a;
                YupperLimit = -b/a;
            }
            int YlowerCoord = std::max(std::max( 0, static_cast<int>(round(YlowerLimit))), std::min(p1.second, p2.second));
            int YupperCoord = std::min( std::min(static_cast<int>(windowSize.second -1), static_cast<int>(round(YupperLimit))), std::max(p1.second, p2.second));
            double x = a*YlowerCoord + b;
            while(YlowerCoord <= YupperCoord){
       		        chartMap[{windowSize.second- (YlowerCoord++)-1,static_cast<int>(round(x))}] = &symbol; 
                x+=a;
            }
        }
    }
}

//DATA ACCESS AND MODIFICATION
std::pair<unsigned, unsigned> Plot::getWindowSize(){
    return windowSize;
}
double Plot::getCellAspectRation(){
    return cellAspectRatio;
}
Scale Plot::getScaling(){
    return scale;
}

void Plot::setWindowSize(std::pair<unsigned, unsigned> WindowSize){
    windowSize = WindowSize;
}
void Plot::setCellAspectratio(double CellAspectRatio){
    cellAspectRatio = CellAspectRatio;

}
void Plot::setScaling(Scale Scale){
    scale = Scale;

}

void Plot::RemoveData(PlotData& removed){
    //remove from reference list
    dataSets.remove_if([&removed](PlotData& data){
        return (&data == &removed);
    });
}

std::tuple<double,double,double,double> Plot::getRange(){
    double min_x = __DBL_MAX__;
    double min_y = __DBL_MAX__;
    double max_x = __DBL_MIN__;
    double max_y = __DBL_MIN__;
    for(PlotData& data : dataSets){
        auto range = data.getRange();
        if (std::isnan(std::get<0>(range)))
            break;
        if(std::get<0>(range) < min_x)
            min_x = std::get<0>(range);
        if(std::get<1>(range) > max_x)
            max_x = std::get<1>(range);
        if(std::get<2>(range) < min_y)
            min_y = std::get<2>(range);
        if(std::get<3>(range) > max_y)
            max_y = std::get<3>(range);
    }
    return std::make_tuple(min_x,max_x,min_y,max_y);
}

//operators
std::ostream& operator<<(std::ostream& s, const Plot& t){ 
    s << t.printableChart;
    s << "\033[39m";
    return s;
}
