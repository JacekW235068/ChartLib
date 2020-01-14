#pragma once

#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
class TextChart
{
//private:
public:
    //DATA
    char** printableData;
    std::pair<unsigned, unsigned> windowSize;
    //symbol used for drawing chart
    char symbol;
    // <x,y> set of data to include
    std::vector<std::pair<double, double>> dataSet;
    double min_y;
    double min_x;
    double max_y;
    double max_x;
    //METHODS
    char** createPrintableData();
    void Draw(std::ostream& stream);
    //CONSTRUCTORS
    TextChart(std::pair<unsigned, unsigned> WindowSize,
        char Symbol,
        std::vector<std::pair<double, double>> dataSet,
        bool Fast);
    ~TextChart();
    
};
