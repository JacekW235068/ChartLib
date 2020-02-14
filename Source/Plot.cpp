 #include "../Header/Plot.hpp"




Plot::Plot(std::pair<unsigned, unsigned> WindowSize,
    Scale Scale,
    double CellAspectRatio
    ) :
    windowSize(WindowSize),
    scale(Scale),
    cellAspectRatio(CellAspectRatio),
    min_x(__DBL_MAX__),
    max_x(__DBL_MIN__),
    min_y(__DBL_MAX__),
    max_y(__DBL_MIN__)

{
    dataSets = std::list<std::reference_wrapper<PlotData>>();
    //create array of chars for chart  
    printableData = new char*[windowSize.second];
    for(int i = 0; i < windowSize.second; i++){
        printableData[i] = new char[windowSize.first];   
    }
}
Plot::~Plot()
{
    for(int i = 0; i < windowSize.second; i++){
        delete printableData[i];
        printableData[i] = nullptr;
    }
    delete printableData;
    printableData = nullptr;
}
void Plot::addDataSet(PlotData& plot){
    dataSets.push_back(plot);
    auto range = plot.getRange();
    if (std::isnan(std::get<0>(range)))
    return;
    if(std::get<0>(range) < min_x)
        min_x = std::get<0>(range);
    if(std::get<1>(range) > max_x)
        max_x = std::get<1>(range);
    if(std::get<2>(range) < min_y)
        min_y = std::get<2>(range);
    if(std::get<3>(range) > max_y)
        max_y = std::get<3>(range);
    
}


void Plot::createChart(double center){
    //calculate visible value range, set limits
    

    switch (scale)
    {
    case Scale::AlignToX:
        if (std::isnan(center))
            center = (max_y+min_y)/2;
        valueRange_scalex(center);
        break;
    case Scale::AlignToY:
        if (std::isnan(center))
            center = (max_x+min_x)/2;
        valueRange_scaley(center);
        break;    
    default:
        valueRange_stretch();
        break;
    }
    //prefill printable array of chars
    for(int i = 0; i < windowSize.second; i++){
        for(int j = 0; j < windowSize.first; j++)
            printableData[i][j] = ' ';
    }
    //draw chart
    for (PlotData& dataSet : dataSets)
    switch (dataSet.style)
    {
    case Style::Linear:
        drawLines(dataSet);
        break;
    default:
        drawDots(dataSet);
        break;
    }
}

void Plot::createChart(std::pair<double,double> Xrange, std::pair<double,double> Yrange){
    //calculate visible value range, set limits
    if (Xrange.first >= Xrange.second || Yrange.first >= Yrange.second)
        return; //should propably throw here
    visible_min_x = Xrange.first;
    visible_max_x = Xrange.second;
    visible_min_y = Yrange.first;
    visible_max_y = Yrange.second;
    //prefill printable array of chars
    for(int i = 0; i < windowSize.second; i++){
        for(int j = 0; j < windowSize.first; j++)
            printableData[i][j] = ' ';
    }
    //draw chart
    for (PlotData& dataSet : dataSets)
    switch (dataSet.style)
    {
    case Style::Linear:
        drawLines(dataSet);
        break;
    default:
        drawDots(dataSet);
        break;
    }
}
void Plot::valueRange_stretch(){
    //set limits
    visible_min_y = min_y;
    visible_min_x = min_x;
    visible_max_y = max_y;
    visible_max_x = max_x;
}

void Plot::valueRange_scaley(double center){
    //range of y axis
    double valueRangeY = abs(max_y - min_y);
    //y range per cell (with cell aspect ratio included) * X cells
    double valueRangeX = valueRangeY/(windowSize.second)*windowSize.first*cellAspectRatio;
    //set limits
    visible_min_y = min_y;
    visible_min_x = center - valueRangeX/2;
    visible_max_y = max_y;
    visible_max_x = center + valueRangeX/2;
}


void Plot::valueRange_scalex(double center){
    //range of x axis
    double valueRangeX = abs(max_x - min_x);
    //x range per cell  * y cells (with cell aspect ratio included)
    double valueRangeY = valueRangeX/(windowSize.first*cellAspectRatio)*windowSize.second;
    //set limits
    visible_min_y = center - valueRangeY/2;
    visible_min_x = min_x;
    visible_max_y = center + valueRangeY/2;
    visible_max_x = max_x;
}

void Plot::drawDots(PlotData& DataSet){
    double visibleRangeX = visible_max_x - visible_min_x;
    double visibleRangeY = visible_max_y - visible_min_y;
    
    //asign each element to place in chart
    for(const auto& data : DataSet.getData({visible_min_x, visible_max_x}, {visible_min_x, visible_max_x})){
        int y = static_cast<int>(round((data.second-visible_min_y)/visibleRangeY*(windowSize.second-1)));
        int x =static_cast<int>(round((data.first-visible_min_x)/visibleRangeX*(windowSize.first-1)));
        //check if in visible range
        if(x >= 0 && y >= 0 && x < windowSize.first && y < windowSize.second)
            printableData[y][x] = DataSet.symbol;
    }
}

void Plot::drawLines(PlotData& plotData){
    double visibleRangeX = visible_max_x - visible_min_x;
    double visibleRangeY = visible_max_y - visible_min_y;
    auto& dataSet = plotData.getData();
    //coords for first point
    std::pair<int,int> previousCoords(
        static_cast<int>(round((dataSet.begin()->first-visible_min_x)/visibleRangeX*(windowSize.first-1))),
        static_cast<int>(round((dataSet.begin()->second-visible_min_y)/visibleRangeY*(windowSize.second-1)))
    );  
    for(std::pair<double,double>& data : dataSet){
        int y = static_cast<int>(round((data.second-visible_min_y)/visibleRangeY*(windowSize.second-1)));
        int x =static_cast<int>(round((data.first-visible_min_x)/visibleRangeX*(windowSize.first-1)));
        drawLine(previousCoords, {x,y}, plotData.symbol);
        //new point becomes old
        previousCoords = {x,y};
    }
}


void Plot::drawLine(std::pair<int, int> p1, std::pair<int,int> p2, char symbol){
    //straight line X
    if (p1.first == p2.first){
        //is line in visible field?
        if (p1.first >= 0 && p1.first < windowSize.first){
            //draw line from low limit to high limit (limits include max window height)
            int YlowwerCoords = std::max(0, std::min(p1.second, p2.second));
            int YupperCoords = std::min(static_cast<int>(windowSize.second-1), std::max(p1.second, p2.second));
            while (YlowwerCoords <= YupperCoords){
                printableData[YlowwerCoords][p1.first] = symbol;
                YlowwerCoords ++;
            } 
        }
    }
    //Same but on y
    else if (p1.second == p2.second){
        if (p1.second >= 0 && p1.second < windowSize.second){
            int XlowwerCoords = std::max(0, std::min(p1.first, p2.first));
            int XupperCoords = std::min(static_cast<int>(windowSize.first-1), std::max(p1.first, p2.first));
            while (XlowwerCoords <= XupperCoords){
                printableData[p1.second][XlowwerCoords] = symbol;
                XlowwerCoords ++;
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
                    printableData[static_cast<int>(round(y))][XlowerCoord++] = symbol;
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
                    printableData[YlowerCoord++][static_cast<int>(round(x))] = symbol;
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
    for(int i = 0; i < windowSize.second; i++){
        printableData[i] = nullptr;
    }
    delete printableData;
    windowSize = WindowSize;
    printableData = new char*[windowSize.second];
    for(int i = 0; i < windowSize.second; i++){
        printableData[i] = new char[windowSize.first];   
    }

}
void Plot::setCellAspectratio(double CellAspectRatio){
    cellAspectRatio = CellAspectRatio;

}
void Plot::setScaling(Scale Scale){
    scale = Scale;

}



//operators
std::ostream& operator<<(std::ostream& s, const Plot& t){ 
    for(int i = t.windowSize.second-1; i >= 0; i--){
        for(int j = 0; j < t.windowSize.first; j++)
            s << t.printableData[i][j];
        s << '\n';
    } 
    return s;
}