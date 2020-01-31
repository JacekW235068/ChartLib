#include "../Header/TextChart.hpp"



TextChart::TextChart(std::pair<unsigned, unsigned> WindowSize,
        std::vector<std::pair<double, double>> &DataSet,
        char Symbol,
        Scale Scale,
        Style Style,
        double CellAspectRatio
        ) : windowSize(WindowSize),
        symbol(Symbol),
        style(Style),
        scale(Scale),
        cellAspectRatio(CellAspectRatio)

{
    //create array of chars for chart  
    printableData = new char*[windowSize.second];
    for(int i = 0; i < windowSize.second; i++){
        printableData[i] = new char[windowSize.first];   
    }
    //fill array with data
    createChart(DataSet);
}

TextChart::~TextChart()
{
        for(int i = 0; i < windowSize.second; i++){
            delete printableData[i];
            printableData[i] = nullptr;
        }
        delete printableData;
        printableData = nullptr;
}

void TextChart::setRange(std::vector<std::pair<double, double>>& dataSet){
    if(dataSet.size() == 0){
        min_y = -1;
        min_x = -1;
        max_y = 1;
        max_x = 1;
    }
    else{
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
    }//add some space for specific cases
    if(min_x == max_x){
        min_x -= 1.0;
        max_x += 1.0;
    }
    if(min_y == max_y){
        min_y -= 1.0;
        max_y += 1.0;
    }
}

char** TextChart::createChart(std::vector<std::pair<double, double>>& dataSet){
    setRange(dataSet);
    std::pair<double,double> range;
    //calculate visible value range, set limits
    switch (scale)
    {
    case Scale::AlignToX:
        range = valueRange_scalex();
        break;
    case Scale::AlignToY:
        range = valueRange_scaley();
        break;    
    default:
        range = valueRange_stretch();
        break;
    }
    //draw chart
    switch (style)
    {
    case Style::Linear:
        drawLines(dataSet, range);
        break;
    default:
        drawDots(dataSet, range);
        break;
    }
    return printableData;
}
std::pair<double, double> TextChart::valueRange_stretch(){
    //visible range of x axis
    double valueRangeX = abs(max_x - min_x);
    //visible range of y axis
    double valueRangeY = abs(max_y - min_y);
    //set limits
    visible_min_y = min_y;
    visible_min_x = min_x;
    visible_max_y = max_y;
    visible_max_x = max_x;
    return std::pair<double,double>(valueRangeX, valueRangeY);
}

std::pair<double, double> TextChart::valueRange_scaley(){
    //range of y axis
    double valueRangeY = abs(max_y - min_y);
    //y range per cell (with cell aspect ratio included) * X cells
    double valueRangeX = valueRangeY/(windowSize.second)*windowSize.first*cellAspectRatio;
    //set limits
    visible_min_y = min_y;
    visible_min_x = (min_x+max_x)/2 - valueRangeX/2;
    visible_max_y = max_y;
    visible_max_x = visible_min_x + valueRangeX;
    return std::pair<double,double>(valueRangeX, valueRangeY);
}


std::pair<double, double> TextChart::valueRange_scalex(){
    //range of x axis
    double valueRangeX = abs(max_x - min_x);
    //x range per cell  * y cells (with cell aspect ratio included)
    double valueRangeY = valueRangeX/(windowSize.first*cellAspectRatio)*windowSize.second;
    //set limits
    visible_min_y = (min_y+max_y)/2 - valueRangeY/2;;
    visible_min_x = min_x;
    visible_max_y = visible_min_y + valueRangeY;
    visible_max_x = max_x;
    return std::pair<double,double>(valueRangeX, valueRangeY);
}

void TextChart::drawDots(std::vector<std::pair<double, double>>& DataSet, std::pair<double, double>& range){
    //prefill printable array of chars
    for(int i = 0; i < windowSize.second; i++){
        for(int j = 0; j < windowSize.first; j++)
            printableData[i][j] = ' ';
    }
    //asign each element to place in chart
    for(const auto& data : DataSet){
        int y = static_cast<int>(round((data.second-visible_min_y)/range.second*(windowSize.second-1)));
        int x =static_cast<int>(round((data.first-visible_min_x)/range.first*(windowSize.first-1)));
        //check if in visible range
        if(x >= 0 && y >= 0 && x < windowSize.first && y < windowSize.second)
            printableData[y][x] = symbol;
    }
}

void TextChart::drawLines(std::vector<std::pair<double, double>>& DataSet, std::pair<double, double>& range){
    //was previous point visible
    bool previous;
    //prefill printable array of chars
    for(int i = 0; i < windowSize.second; i++){
        for(int j = 0; j < windowSize.first; j++)
            printableData[i][j] = ' ';
    }
    //coords for first point skipped in loop
    std::pair<int,int> previousCoords(
        static_cast<int>(round((DataSet.begin()->first-visible_min_x)/range.first*(windowSize.first-1))),
        static_cast<int>(round((DataSet.begin()->second-visible_min_y)/range.second*(windowSize.second-1)))
    );
    previous = (previousCoords.first >= 0 && previousCoords.second >= 0 &&
        previousCoords.first < windowSize.first && previousCoords.second < windowSize.second);

    std::for_each(++DataSet.begin(), DataSet.end(), [&](std::pair<double,double>& data){
        //coords of new point
        int y = static_cast<int>(round((data.second-visible_min_y)/range.second*(windowSize.second-1)));
        int x =static_cast<int>(round((data.first-visible_min_x)/range.first*(windowSize.first-1)));
        //if old or new is visible draw line
        if(x >= 0 && y >= 0 && x < windowSize.first && y < windowSize.second || previous)
            drawLine(previousCoords, {x,y});
        //new point becomes old
        if(x >= 0 && y >= 0 && x < windowSize.first && y < windowSize.second)
            previous = true;
        previousCoords = {x,y};
    });
}


void TextChart::drawLine(std::pair<int, int> p1, std::pair<int,int> p2){
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
            int x = std::min(p1.first, p2.first);
            double y = a*x + b;
            //put in array if it fits
            for(x; x <= std::max(p1.first, p2.first); x++ ){
                if(x >= 0 && x < windowSize.first &&
                y >= 0 && static_cast<int>(round(y)) < windowSize.second)
                    printableData[static_cast<int>(round(y))][x] = symbol;
                y+=a;
            }
        }else{
            //same as above exept with this formula
            //x = ay + b
            double a = static_cast<double>(p2.first - p1.first) / (p2.second - p1.second);
            double b = static_cast<double>(p1.first) - (a*p1.second);
            int y = std::min(p1.second, p2.second);
            double x = a*y+b;
            for(y; y <= std::max(p1.second, p2.second); y++ ){
                if(x >= 0 && static_cast<int>(round(x)) < windowSize.first &&
                y >= 0 && y < windowSize.second)
                    printableData[y][static_cast<int>(round(x))] = symbol;
                x+=a;
            }
        }
    }
}

//operators
std::ostream& operator<<(std::ostream& s, const TextChart& t){ 
    for(int i = t.windowSize.second-1; i >= 0; i--){
        for(int j = 0; j < t.windowSize.first; j++)
            s << t.printableData[i][j];
        s << '\n';
    } 
    return s;
}