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
}

void Plot::setRange(){
    //x min, max, y min,max
    auto dataRange = std::make_tuple(__DBL_MAX__,__DBL_MIN__,__DBL_MAX__,__DBL_MIN__);
    for(PlotData& data : dataSets){
        auto range = data.getRange();
        if (std::isnan(std::get<0>(range)))
            break;
        if(std::get<0>(range) < std::get<0>(dataRange))
            std::get<0>(dataRange) = std::get<0>(range);
        if(std::get<1>(range) > std::get<1>(dataRange))
            std::get<1>(dataRange) = std::get<1>(range);
        if(std::get<2>(range) < std::get<2>(dataRange))
            std::get<2>(dataRange) = std::get<2>(range);
        if(std::get<3>(range) > std::get<3>(dataRange))
            std::get<3>(dataRange) = std::get<3>(range);
    }
    min_x = std::get<0>(dataRange);
    max_x = std::get<1>(dataRange);
    min_y = std::get<2>(dataRange);
    max_y = std::get<3>(dataRange);
}


void Plot::createChart(std::pair<double,double> Xrange, std::pair<double,double> Yrange){
    //calculate visible value range, set limits
    if (Xrange.first > Xrange.second || Yrange.first > Yrange.second)
        switch (scale)
        {
        case Scale::AlignToX:
            valueRange_scalex();
            break;
        case Scale::AlignToY:
            valueRange_scaley();
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
std::pair<double, double> Plot::valueRange_stretch(){
    //visible range of x axis
    double valueRangeX = abs(max_x - min_x);
    //visible range of y axis
    double valueRangeY = abs(max_y - min_y);
    //set limits
    visible_min_y = min_y;
    visible_min_x = min_x;
    visible_max_y = max_y;
    visible_max_x = max_x;
}

std::pair<double, double> Plot::valueRange_scaley(){
    //range of y axis
    double valueRangeY = abs(max_y - min_y);
    //y range per cell (with cell aspect ratio included) * X cells
    double valueRangeX = valueRangeY/(windowSize.second)*windowSize.first*cellAspectRatio;
    //set limits
    visible_min_y = min_y;
    visible_min_x = (min_x+max_x)/2 - valueRangeX/2;
    visible_max_y = max_y;
    visible_max_x = visible_min_x + valueRangeX;
}


std::pair<double, double> Plot::valueRange_scalex(){
    //range of x axis
    double valueRangeX = abs(max_x - min_x);
    //x range per cell  * y cells (with cell aspect ratio included)
    double valueRangeY = valueRangeX/(windowSize.first*cellAspectRatio)*windowSize.second;
    //set limits
    visible_min_y = (min_y+max_y)/2 - valueRangeY/2;;
    visible_min_x = min_x;
    visible_max_y = visible_min_y + valueRangeY;
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
    //coords for first point
    auto& dataSet = plotData.getData();
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
    //straight line cases
    if (p1.first == p2.first){
        for(int i = std::min(p1.second, p2.second); i <= std::max(p1.second, p2.second); i++)
            if(p1.first >= 0 && p1.first < windowSize.first &&
            i >= 0 && i < windowSize.second)
                printableData[i][p1.first] = symbol;
    }
    else if (p1.second == p2.second){
        for(int i = std::min(p1.first, p1.first); i <= std::max(p1.first, p1.first); i++)
            if(p1.second >= 0 && p1.second < windowSize.second &&
            i >= 0 && i < windowSize.first)
                printableData[p1.second][i] = symbol;
    }
    else{
        unsigned x_length = abs(p1.first - p2.first);
        unsigned y_length = abs(p1.second - p2.second);
        //pick one providing higher resolution so it fills everything between two points
        if(x_length > y_length){
            //y=ax+b
            double a = static_cast<double>(p2.second - p1.second) / (p2.first - p1.first);
            double b = static_cast<double>(p1.second) - (a*p1.first);
            double XlowerLimit;
            double XupperLimit;
            if(a > 0){
                XlowerLimit = -b/a;
                XupperLimit = (windowSize.second -1 - b)/a;
            }else{
                XlowerLimit = (windowSize.second -1 - b)/a;
                XupperLimit = -b/a;
            }
            int XlowerCoord = std::max(std::max( 0, static_cast<int>(round(XlowerLimit))), std::min(p1.first, p2.first));
            int XupperCoord = std::min( std::min(static_cast<int>(windowSize.first -1), static_cast<int>(round(XupperLimit))), std::max(p1.first, p2.first));
            double y = a*XlowerCoord + b;
            //put in array if it fits
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
            int YlowerCoord = std::max(std::max( 0, static_cast<int>(round(YlowerLimit))), std::min(p1.first, p2.first));
            int YupperCoord = std::min( std::min(static_cast<int>(windowSize.second -1), static_cast<int>(round(YupperLimit))), std::max(p1.first, p2.first));
            double x = a*YlowerCoord + b;
            //put in array if it fits
            while(YlowerCoord <= YupperCoord){
                    printableData[YlowerCoord++][static_cast<int>(round(x))] = symbol;
                x+=a;
            }
        }
    }
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