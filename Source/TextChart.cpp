#include "../Headers/TextChart.hpp"



TextChart::TextChart(std::pair<unsigned, unsigned> WindowSize,
        char Symbol,
        std::vector<std::pair<double, double>> DataSet,
        bool Fast) 
            : windowSize(WindowSize),
            symbol(Symbol),
            dataSet(DataSet)
{
    printableData = nullptr;
}

TextChart::~TextChart()
{
}

char** TextChart::createPrintableData(){
    if(printableData != nullptr){
        for(int i = 0; i < windowSize.second; i++){
            delete printableData[i];
        delete printableData;
    }
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
    //visible range of x axis
    double valueRangeX = abs(max_x - min_x);
    //visible range of y axis
    double valueRangeY = abs(max_y - min_y);
    printableData = new char*[windowSize.second];
    //create and prefill printable array of chars
    for(int i = 0; i < windowSize.second; i++){
        printableData[i] = new char[windowSize.first];
        for(int j = 0; j < windowSize.first; j++)
            printableData[i][j] = ' ';
    }
    //asign each element to place in chart
    for(const auto& data : dataSet){
        int y = static_cast<int>(abs(data.second-min_y)/valueRangeY*(windowSize.second-1));
        int x =static_cast<int>(abs(data.first-min_x)/valueRangeX*(windowSize.first-1));
        printableData
            [y]
            [x] = symbol;
    }
    return printableData;
}
void TextChart::Draw(std::ostream& stream){
    std::string output;
    for(int i = windowSize.second-1; i >= 0; i--){
        for(int j = 0; j < windowSize.first; j++)
            output += printableData[i][j];
        output += '\n';
    }
    stream << output;
}