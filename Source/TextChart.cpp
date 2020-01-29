#include "../Header/TextChart.hpp"



TextChart::TextChart(std::pair<unsigned, unsigned> WindowSize,
        std::vector<std::pair<double, double>> DataSet,
        char Symbol,
        Scale Scale,
        Linearity  Linearity,
        double CellAspectRatio
        ) : windowSize(WindowSize),
        symbol(Symbol),
        dataSet(DataSet),
        linearity(Linearity),
        scale(Scale),
        cellAspectRatio(CellAspectRatio)

{
    printableData = new char*[windowSize.second];
    for(int i = 0; i < windowSize.second; i++){
        printableData[i] = new char[windowSize.first];   
    }
    setRange();
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

void TextChart::setRange(){
    if(dataSet.size() == 0){
        min_y = -1;
        min_x = -1;
        max_y = 1;
        max_x = 1;
    }
    else if(dataSet.size() == 1){
        auto& element = *dataSet.begin();
        min_y = element.second-1;
        min_x = element.first -1;
        max_y = element.second+1;
        max_x = element.first +1;
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
    }
}

char** TextChart::createPrintableData(){
    //calculate visible range for axis
    std::pair<double,double> range;
    switch (scale)
    {
    case Scale::AlignToX:
        range = valueRange_scalex();
        break;
    case Scale::AlignToy:
        range = valueRange_scaley();
        break;    
    default:
        range = valueRange_stretch();
        break;
    }
    //create and prefill printable array of chars
    for(int i = 0; i < windowSize.second; i++){
        for(int j = 0; j < windowSize.first; j++)
            printableData[i][j] = ' ';
    }
    //asign each element to place in chart
    for(const auto& data : dataSet){
        int y = static_cast<int>(round((data.second-visible_min_y)/range.second*(windowSize.second-1)));
        int x =static_cast<int>(round((data.first-visible_min_x)/range.first*(windowSize.first-1)));
        //check if in visible range
        if(x >= 0 && y >= 0 && x < windowSize.first && y < windowSize.second)
            printableData[y][x] = symbol;
    }
    return printableData;
}
void TextChart::Draw(std::ostream& stream){
    std::string output;
    output += "start\n"; 
    for(int i = windowSize.second-1; i >= 0; i--){
        for(int j = 0; j < windowSize.first; j++)
            output += printableData[i][j];
        output += '\n';
    }
    output += "end\n"; 
    stream << output;
}
std::pair<double, double> TextChart::valueRange_stretch(){
    //range of x axis
    double valueRangeX = abs(max_x - min_x);
    //range of y axis
    double valueRangeY = abs(max_y - min_y);
    //adjust visible range
    visible_min_y = min_y;
    visible_min_x = min_x;
    visible_max_y = max_y;
    visible_max_x = max_x;
    return std::pair<double,double>(valueRangeX, valueRangeY);
}

std::pair<double, double> TextChart::valueRange_scaley(){
    //range of y axis
    double valueRangeY = abs(max_y - min_y);
    //Y range per cell (with cell aspect ratio included) * X cells
    double valueRangeX = valueRangeY/(windowSize.second)*windowSize.first*cellAspectRatio;
    //adjust visible range
    visible_min_y = min_y;
    visible_min_x = (min_x+max_x)/2 - valueRangeX/2;
    visible_max_y = max_y;
    visible_max_x = visible_min_x + valueRangeX;
    return std::pair<double,double>(valueRangeX, valueRangeY);
}


std::pair<double, double> TextChart::valueRange_scalex(){
    //range of x axis
    double valueRangeX = abs(max_x - min_x);
    //range of y axis
    double valueRangeY = valueRangeX/(windowSize.first*cellAspectRatio)*windowSize.second;
    //adjust visible range
    visible_min_y = (min_y+max_y)/2 - valueRangeY/2;;
    visible_min_x = min_x;
    visible_max_y = visible_min_y + valueRangeY;
    visible_max_x = max_x;
    return std::pair<double,double>(valueRangeX, valueRangeY);
}