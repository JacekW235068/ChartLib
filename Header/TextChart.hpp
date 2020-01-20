#pragma once

#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>

#include "Enum/Scale.cpp"
#include "Enum/Linearity.cpp"


class TextChart
{
//protected:
public:
    //Array of printable data
    char** printableData;
    std::pair<unsigned, unsigned> windowSize;
    //symbol used for drawing chart
    char symbol;
    // <x,y> set of data to include
    std::vector<std::pair<double, double>> dataSet;
    //range of chart
    double min_y;
    double min_x;
    double max_y;
    double max_x;
    //METHODS
    char** createPrintableData();
    void Draw(std::ostream& stream);
    //CONSTRUCTORS
    TextChart(std::pair<unsigned, unsigned> WindowSize,
        std::vector<std::pair<double, double>> DataSet,
        char Symbol = 'o',
        Scale Scale = Scale::stretch,
        Linearity  Linearity = Linearity::Dots 
        );
    ~TextChart();
    
};
