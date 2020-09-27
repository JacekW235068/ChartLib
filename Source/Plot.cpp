#include <Plot.hpp>

#include <memory>
#include <algorithm>

namespace chart {
Plot::Plot(std::pair<uint16_t, uint16_t> WindowSize,
    double CellAspectRatio
    ) :
    windowSize(WindowSize),
    cellAspectRatio(CellAspectRatio),
    visible_min_x(nan("")),
    visible_max_x(nan("")),
    visible_min_y(nan("")),
    visible_max_y(nan(""))
{
}

Plot::~Plot()
{
    for (PlotData* data : dataSets){
        data->plots.remove(this);
    }
}


void Plot::addDataSet(PlotData* plotData){
    if (std::find(dataSets.begin(),dataSets.end(),plotData) != dataSets.end())
        return;
    dataSets.push_back(plotData);
    plotData->plots.push_back(this);

}

void Plot::addDataSet(std::vector<PlotData*>& plotData){
    for (PlotData* dataSet : plotData){
        if (std::find(dataSets.begin(),dataSets.end(),dataSet) != dataSets.end())
            continue;
        dataSets.push_back(dataSet);
        dataSet->plots.push_back(this);

    }
}

void Plot::addDataSet(std::list<PlotData*>& plotData){
    for (PlotData* dataSet : plotData){
        if (std::find(dataSets.begin(),dataSets.end(),dataSet) != dataSets.end())
            continue;
        dataSets.push_back(dataSet);
        dataSet->plots.push_back(this);
    }
}

void Plot::setVisibleRange(std::pair<double,double> Xrange, std::pair<double,double> Yrange){
    visible_min_x = Xrange.first;
    visible_max_x = Xrange.second;
    visible_min_y = Yrange.first;
    visible_max_y = Yrange.second;
}

void Plot::setVisibleRange(Scale scaling, double center){
    switch (scaling)
    {
    case Scale::AlignToX:
        visibleRange_scalex(center);
        break;
    case Scale::AlignToY:
        visibleRange_scaley(center);
        break;    
    case Scale::stretch:
        visibleRange_stretch();
        break;
    }
}

void Plot::visibleRange_stretch(){
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

void Plot::visibleRange_scaley(double center){
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


void Plot::visibleRange_scalex(double center){
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

void Plot::drawDots(PlotData& DataSet){
    if (std::isnan(visible_min_x) or std::isnan(visible_max_x) or std::isnan(visible_min_y) or std::isnan(visible_max_y))
        return;
    double visibleRangeX = visible_max_x - visible_min_x;
    double visibleRangeY = visible_max_y - visible_min_y;
    
    //asign each element to place in chart
    std::list< std::pair<double, double> > listElements = DataSet.getData();

    for(const auto& data : DataSet.getData()){
        if(data.first >= visible_min_x && data.first <= visible_max_x &&
        data.second >= visible_min_y && data.second <= visible_max_y ){
            int y = static_cast<int>(round((visible_max_y - data.second)/visibleRangeY*(windowSize.second-1)));
            int x = static_cast<int>(round((data.first-visible_min_x)/visibleRangeX*(windowSize.first-1)));
            ChartMap[{y, x}] = DataSet.getStyledSymbol();
        }
    }
}

void Plot::drawLines(PlotData& plotData){
    if (std::isnan(visible_min_x) or std::isnan(visible_max_x) or std::isnan(visible_min_y) or std::isnan(visible_max_y))
        return;
    double visibleRangeX = visible_max_x - visible_min_x;
    double visibleRangeY = visible_max_y - visible_min_y;
    auto& dataSet = plotData.getData();
    //coords for first point
    std::pair<long,long> previousCoords(
        static_cast<long>(round((dataSet.begin()->first-visible_min_x)/visibleRangeX*(windowSize.first-1))),
        static_cast<long>(round((visible_max_y - dataSet.begin()->second)/visibleRangeY*(windowSize.second-1)))
    );  
    for(const std::pair<double,double>& data : dataSet){
        long y = static_cast<long>(round((visible_max_y - data.second)/visibleRangeY*(windowSize.second-1)));
        long x =static_cast<long>(round((data.first-visible_min_x)/visibleRangeX*(windowSize.first-1)));
        drawLine(previousCoords, {x,y}, plotData.getStyledSymbol());
        //new polong becomes old
        previousCoords = {x,y};
    }
}

void Plot::drawLine(std::pair<long, long> p1, std::pair<long,long> p2,const std::string &symbol){
    //straight line X
    if (p1.first == p2.first){
        //is line in visible field?
        if (p1.first >= 0 && p1.first < windowSize.first){
            //draw line from low limit to high limit (limits include max window height)
            long YlowerCoords = std::max(0L, std::min(p1.second, p2.second));
            long YupperCoords = std::min(static_cast<long>(windowSize.second-1), std::max(p1.second, p2.second));
            while (YlowerCoords <= YupperCoords){
                ChartMap[{YlowerCoords, p1.first}] = symbol;
                YlowerCoords ++;
            }
        }
    }
    //Same but on y
    else if (p1.second == p2.second){
        if (p1.second >= 0 && p1.second < windowSize.second){
            long XlowerCoords = std::max(0L, std::min(p1.first, p2.first));
            long XupperCoords = std::min(static_cast<long>(windowSize.first-1), std::max(p1.first, p2.first));
            while (XlowerCoords <= XupperCoords){
       		    ChartMap[{p1.second, XlowerCoords}] = symbol; 
                XlowerCoords ++;
            }
        } 
    }
    else{
        unsigned x_length = abs(p1.first - p2.first);
        unsigned y_length = abs(p1.second - p2.second);
        //pick one with higher 'resolution' so it fills everything between two polongs
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
            //compare calculated limits with visible window limits and polong start,end coords
            long XlowerCoord = std::max(std::max( 0L, static_cast<long>(round(XlowerLimit))), std::min(p1.first, p2.first));
            long XupperCoord = std::min( std::min(static_cast<long>(windowSize.first -1), static_cast<long>(round(XupperLimit))), std::max(p1.first, p2.first));
            //y calculated as function of x
            double y = a*XlowerCoord + b;
            while(XlowerCoord <= XupperCoord){
       		        ChartMap[{static_cast<long>(round(y)), XlowerCoord++}] = symbol; 
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
            long YlowerCoord = std::max(std::max( 0L, static_cast<long>(round(YlowerLimit))), std::min(p1.second, p2.second));
            long YupperCoord = std::min( std::min(static_cast<long>(windowSize.second -1), static_cast<long>(round(YupperLimit))), std::max(p1.second, p2.second));
            double x = a*YlowerCoord + b;
            while(YlowerCoord <= YupperCoord){
       		        ChartMap[{(YlowerCoord++),static_cast<long>(round(x))}] = symbol; 
                x+=a;
            }
        }
    }
}

//DATA ACCESS AND MODIFICATION
const std::pair<uint16_t, uint16_t>& Plot::getWindowSize() const{
    return windowSize;
}

const double& Plot::getCellAspectRatio() const{
    return cellAspectRatio;
}

void Plot::removeDataSet(PlotData* removed){
    dataSets.remove(removed);
    removed->plots.remove(this);
}



std::tuple<double,double,double,double> Plot::getRange(){
    double min_x = __DBL_MAX__;
    double min_y = __DBL_MAX__;
    double max_x = __DBL_MIN__;
    double max_y = __DBL_MIN__;
    for(PlotData* data : dataSets){
        auto range = data->getRange();
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

std::tuple<int,int,int,int> Plot::generate(){
    int min_x = 0;
    int min_y = 0;
    int max_y = windowSize.second-1;
    int max_x = windowSize.first-1;

    for (IDecoration* decoration : decorations)
        if(!decoration->isForced()){
            auto [x,X,y,Y] = decoration->drawFrame(ChartMap,
            windowSize,
            {visible_min_x,visible_max_x,visible_min_y,visible_max_y});
            if (x < min_x)
                min_x = x;
            if (y < min_y)
                min_y = y;
            if (X > max_x)
                max_x = X;
            if (Y > max_y)
                max_y = Y;
        }
    for (auto dataSet : dataSets)
        drawOnChartMap(*dataSet);
    for (IDecoration* decoration : decorations)
        if(decoration->isForced()){
            auto [x,X,y,Y] = decoration->drawFrame(ChartMap,
            windowSize,
            {visible_min_x,visible_max_x,visible_min_y,visible_max_y});
            if (x < min_x)
                min_x = x;
            if (y < min_y)
                min_y = y;
            if (X > max_x)
                max_x = X;
            if (Y > max_y)
                max_y = Y;
        }
    return {min_x,max_x,min_y,max_y};
}

std::string Plot::print(){
    auto [min_x,max_x,min_y,max_y] = generate();
    std::string result;
    result.reserve(windowSize.first*windowSize.second+21);
    auto plot_it = ChartMap.begin();
    for(int line=min_y;line <= max_y; line++){
        int x = min_x-1;
        while( plot_it != ChartMap.end() && (*plot_it).first.first == line){
            //fill spaces + place symbol + default color
            result += std::string((*plot_it).first.second-x-1, ' ') + (*plot_it).second + "\033[39m";
            x = (*plot_it++).first.second;
        }
        result += std::string(max_x-x,' ');
        result += '\n';
    }
    ChartMap.clear();
    return result;
}

std::string Plot::getLegend(){
    std::string Legend;
    //Totally readable code
    auto x = *(std::max_element(dataSets.begin(),dataSets.end(),
    [](const PlotData* a, const PlotData* b){
        return a->getName().length() <  b->getName().length();
    }));
    auto Max = x->getName().length();
    for (PlotData* dataSet : dataSets){
        Legend += dataSet->getName() + std::string(Max - dataSet->getName().length(),'.') + dataSet->getStyledSymbol() + "\033[39m\n";
    }
    return Legend;
}

void Plot::drawOnChartMap(PlotData& plotData){
    switch (plotData.style)
    {
        case Style::Linear:
            drawLines(plotData);
            break;
        case Style::dots:
            drawDots(plotData);
            break;
        default: break;
    }
}


void Plot::addDecoration(IDecoration* decoration){
    decorations.push_back(decoration);
}

void Plot::addDecoration(std::vector<IDecoration*> decorations){
    for(IDecoration* decoration : decorations){
        this->decorations.push_back(decoration);
    }
}

void Plot::addDecoration(std::list<IDecoration*> decorations){
    for(IDecoration* decoration : decorations){
        this->decorations.push_back(decoration);
    }
}

void Plot::removeDecoration(IDecoration* decoration){
    decorations.remove(decoration);
}

}
