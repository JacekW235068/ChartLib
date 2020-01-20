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
    printableData = nullptr;
}

TextChart::~TextChart()
{
    if(printableData != nullptr){
        for(int i = 0; i < windowSize.second; i++){
            delete printableData[i];
        }
        delete printableData;
    }
}

char** TextChart::createPrintableData(){
    if(printableData != nullptr){
        for(int i = 0; i < windowSize.second; i++){
            delete printableData[i];
        }
        delete printableData;
    }
    //Find min and max values in dataset 
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
    printableData = new char*[windowSize.second];
    for(int i = 0; i < windowSize.second; i++){
        printableData[i] = new char[windowSize.first];
        for(int j = 0; j < windowSize.first; j++)
            printableData[i][j] = ' ';
    }
    //asign each element to place in chart
    for(const auto& data : dataSet){
        int y = static_cast<int>(abs(data.second-min_y)/range.second*(windowSize.second-1));
        int x =static_cast<int>(abs(data.first-min_x)/range.first*(windowSize.first-1));
        printableData
            [y]
            [x] = symbol;
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
    return std::pair<double,double>(valueRangeX, valueRangeY);
}

//TODO
std::pair<double, double> TextChart::valueRange_scaley(){
    //range of x axis
    double valueRangeX = abs(max_x - min_x);
    //range of y axis
    double valueRangeY = abs(max_y - min_y);
    return std::pair<double,double>(valueRangeX, valueRangeY);
}

//TODO
std::pair<double, double> TextChart::valueRange_scalex(){
    //range of x axis
    double valueRangeX = abs(max_x - min_x);
    //range of y axis
    double valueRangeY = abs(max_y - min_y);
    return std::pair<double,double>(valueRangeX, valueRangeY);
}