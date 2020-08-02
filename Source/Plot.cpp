 #include <Plot.hpp>



namespace chart {
Plot::Plot(std::pair<unsigned, unsigned> WindowSize,
    double CellAspectRatio
    ) :
    windowSize(WindowSize),
    cellAspectRatio(CellAspectRatio),
    visible_min_x(nan("")),
    visible_max_x(nan("")),
    visible_min_y(nan("")),
    visible_max_y(nan(""))
{
    dataSets = std::list<std::reference_wrapper<PlotData>>();
}
Plot::~Plot()
{
}
void Plot::addDataSet(PlotData& plotData){
    if (find_if(dataSets.begin(),dataSets.end(),[&plotData](PlotData& data){
        return (&data == &plotData);
    }) != dataSets.end())
        return;
    dataSets.push_back(plotData);
    plotData.plots.push_back(*this);
    switch (plotData.style)
    {
        case Style::Linear:
            drawLines(plotData);
            break;
        default:
            drawDots(plotData);
            break;
    }
}
void Plot::addDataSets(std::vector<std::reference_wrapper<PlotData>>& plotData){
    for (PlotData& dataSet : plotData){
        if (find_if(dataSets.begin(),dataSets.end(),[&dataSet](PlotData& data){
            return (&data == &dataSet);
            }) != dataSets.end())
            continue;
        dataSets.push_back(dataSet);
        dataSet.plots.push_back(*this);
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
}
void Plot::addDataSets(std::list<std::reference_wrapper<PlotData>>& plotData){
    for (PlotData& dataSet : plotData){
        if (find_if(dataSets.begin(),dataSets.end(),[&dataSet](PlotData& data){
            return (&data == &dataSet);
            }) != dataSets.end())
            continue;
        dataSets.push_back(dataSet);
        dataSet.plots.push_back(*this);
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
}
void Plot::addDataSets(std::vector<PlotData*>& plotData){
    for (PlotData* dataSet : plotData){
        if (find_if(dataSets.begin(),dataSets.end(),[dataSet](PlotData& data){
            return (&data == dataSet);
            }) != dataSets.end())
            continue;
        dataSets.push_back(*dataSet);
        dataSet->plots.push_back(*this);
        switch (dataSet->style)
        {
            case Style::Linear:
                drawLines(*dataSet);
                break;
            default:
                drawDots(*dataSet);
                break;
        }
    }
}
void Plot::addDataSets(std::list<PlotData*>& plotData){
    for (PlotData* dataSet : plotData){
        if (find_if(dataSets.begin(),dataSets.end(),[dataSet](PlotData& data){
            return (&data == dataSet);
            }) != dataSets.end())
            continue;
        dataSets.push_back(*dataSet);
        dataSet->plots.push_back(*this);
        switch (dataSet->style)
        {
            case Style::Linear:
                drawLines(*dataSet);
                break;
            default:
                drawDots(*dataSet);
                break;
        }
    }
}

void Plot::setVisibleRange(std::pair<double,double> Xrange, std::pair<double,double> Yrange){
    visible_min_x = Xrange.first;
    visible_max_x = Xrange.second;
    visible_min_y = Yrange.first;
    visible_max_y = Yrange.second;

    for (PlotData& dataSet : dataSets){
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

    for (PlotData& dataSet : dataSets){
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
            int x =static_cast<int>(round((data.first-visible_min_x)/visibleRangeX*(windowSize.first-1)));
            ChartMap[{y, x}] = &DataSet.getStyledSymbol();
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
    std::pair<int,int> previousCoords(
        static_cast<int>(round((dataSet.begin()->first-visible_min_x)/visibleRangeX*(windowSize.first-1))),
        static_cast<int>(round((visible_max_y - dataSet.begin()->second)/visibleRangeY*(windowSize.second-1)))
    );  
    for(const std::pair<double,double>& data : dataSet){
        int y = static_cast<int>(round((visible_max_y - data.second)/visibleRangeY*(windowSize.second-1)));
        int x =static_cast<int>(round((data.first-visible_min_x)/visibleRangeX*(windowSize.first-1)));
        drawLine(previousCoords, {x,y}, plotData.getStyledSymbol());
        //new point becomes old
        previousCoords = {x,y};
    }
}


void Plot::drawLine(std::pair<int, int> p1, std::pair<int,int> p2,const std::string &symbol){
    //straight line X
    if (p1.first == p2.first){
        //is line in visible field?
        if (p1.first >= 0 && p1.first < windowSize.first){
            //draw line from low limit to high limit (limits include max window height)
            int YlowerCoords = std::max(0, std::min(p1.second, p2.second));
            int YupperCoords = std::min(static_cast<int>(windowSize.second-1), std::max(p1.second, p2.second));
            while (YlowerCoords <= YupperCoords){
       		    ChartMap[{YlowerCoords, p1.first}] = &symbol;
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
       		    ChartMap[{p1.second, XlowerCoords}] = &symbol; 
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
       		        ChartMap[{static_cast<int>(round(y)), XlowerCoord++}] = &symbol; 
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
       		        ChartMap[{(YlowerCoord++),static_cast<int>(round(x))}] = &symbol; 
                x+=a;
            }
        }
    }
}

//DATA ACCESS AND MODIFICATION
const std::pair<unsigned, unsigned>& Plot::getWindowSize() const{
    return windowSize;
}
const double& Plot::getCellAspectRatio() const{
    return cellAspectRatio;
}

void Plot::removeDataSet(PlotData& removed){
    auto it = ChartMap.begin();
	while (it != ChartMap.end())
	{
		if (it->second == &removed.getStyledSymbol())
		{
			it = ChartMap.erase(it);
		}
		else {
			++it;
		}
	}
    dataSets.remove_if([&removed](PlotData& data){
        return (&data == &removed);
    });
    removed.plots.remove_if([&](Plot& plot){
        return (&plot == this);
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



void Plot::noFrame(){
    frame.clear();
    //top
    frame.push_front("");
    for(int i=0; i < windowSize.second; i++){
        frame.push_back("");//prefix
        frame.push_back("");//suffix
    }
    //bottom
    frame.push_back("");
}

void Plot::simpleFrame(){
    frame.clear();
    std::string top= u8"╔";
    std::string bottom = u8"╚";
    for (int i=0; i < windowSize.first; i++){
        bottom += u8"═";
        top += u8"═";
    }
    top+= u8"╗\n";
    bottom += u8"╝";
    //top
    frame.push_back(top);
    for(int i=0; i < windowSize.second; i++){
        frame.push_back(u8"║");//prefix
        frame.push_back(u8"║");//suffix
    }
    //bottom
    frame.push_back(bottom);
}
void Plot::axisFrame(int Xprecission, int Yprecission){
    frame.clear();
    //XAXIS (top&bottom)
    double visibleRangeX = visible_max_x - visible_min_x;
    double xUnit = pow(10.0,Xprecission);
    double x = ceil(visible_min_x/xUnit)*xUnit;
    std::string xAxisMarks = std::string(windowSize.first+1,' ');
    std::string xAxis = "";
    int preAxisCoord = -1;
    std::string number;
    while (x <= visible_max_x){
        x = round(x*pow(10.0,-Xprecission))/pow(10.0,-Xprecission);
        int axisCoord = static_cast<int>(round((x-visible_min_x)/visibleRangeX*(windowSize.first-1)));
        if(axisCoord -preAxisCoord -1<0){
            x+= pow(10.0,Xprecission);
            continue;
        }
        xAxisMarks[axisCoord+1] = '.';
        xAxis += std::string(axisCoord -preAxisCoord -1, ' ');  
        number = std::to_string(x);  
        if(Xprecission >=0){   
            number = number.substr(0,number.find('.'));
        }else
            number = number.substr(0,number.find('.')-Xprecission+1);
        xAxis += number+' ';
        preAxisCoord = xAxis.length()-1;
        x+= pow(10.0,Xprecission);
    }
    xAxisMarks += '\n';
    frame.push_back(xAxisMarks);
    //YAXIS
    double visibleRangeY = visible_max_y - visible_min_y;
    double yUnit = pow(10.0,Yprecission);
    double y = floor(visible_max_y/yUnit)*yUnit;
    y = round(y*pow(10.0,-Yprecission))/pow(10.0,-Yprecission);
    int axisCoord = static_cast<int>(round((visible_max_y - y)/visibleRangeY*(windowSize.second-1)));
    int lineCounter = 0;
    for(int i=0; i < windowSize.second; i++){
        if(axisCoord == lineCounter){
            number = std::to_string(y);  
            if(Yprecission >=0){   
                number = number.substr(0,number.find('.'));
            }else
                number = number.substr(0,number.find('.')-Yprecission+1);
            frame.push_back("-");
            frame.push_back("-"+number);
            while (axisCoord==lineCounter){
                y-= pow(10.0,Yprecission);
                y = round(y*pow(10.0,-Yprecission))/pow(10.0,-Yprecission);
                axisCoord = static_cast<int>(round((visible_max_y - y)/visibleRangeY*(windowSize.second-1)));
            }
        }else{
            frame.push_back(" ");
            frame.push_back("");
        }
        lineCounter++;
    }
    frame.push_back(xAxisMarks+xAxis);
}


//operators
std::ostream& operator<<(std::ostream& s, const Plot& t){ 
    auto map_it = t.ChartMap.begin();
    auto frame_it = t.frame.begin();
    s <<*frame_it++;
    for(int line=0;line < t.windowSize.second;line++){
        int x = -1;
        s <<*frame_it++;
        while( map_it != t.ChartMap.end() && (*map_it).first.first == line){
            //fill spaces
            s << std::string((*map_it).first.second-x-1, ' ');
            //place symbol
            s << *(*map_it).second;
            x = (*map_it).first.second;
            map_it++;
        }
        s << "\033[39m";
        s << std::string(t.windowSize.first -x-1,' ');
        s <<*frame_it++;
        s<< '\n';
    }
    s <<*frame_it++;
    s << "\033[39m";
    s<<'\n';
    return s;
}

std::string Plot::getLegend(){
    std::string Legend;
    //Totally readable code
    int MaxWordLength = std::max_element(dataSets.begin(),dataSets.end(),
    [](const PlotData& a, const PlotData& b){
        return a.getName().length() <  b.getName().length();
    })->get().getName().length() + 3;
    for (PlotData& dataSet : dataSets){
        Legend += dataSet.getName() + std::string(MaxWordLength - dataSet.getName().length(),'.') + dataSet.getStyledSymbol() + "\033[39m\n";
    }
    return Legend;
}
}