#pragma once

#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

#include "../Enum/Scale.cpp"
#include "../Enum/Linearity.cpp"


class TextChart
{
//protected:
public:
    //chart settings
    Linearity linearity;
    Scale scale;
    //terminal character cell aspect ratio X/Y
    double cellAspectRatio;
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
    double visible_min_y;
    double visible_min_x;
    double visible_max_y;
    double visible_max_x;
    //METHODS
    char** createPrintableData();
    std::pair<double, double> valueRange_scalex();
    std::pair<double, double> valueRange_scaley();
    std::pair<double, double> valueRange_stretch();
    void Draw(std::ostream& stream);
    //set default range
    void setRange();
    //CONSTRUCTORS
    TextChart(std::pair<unsigned, unsigned> WindowSize,
        std::vector<std::pair<double, double>> DataSet,
        char Symbol = 'o',
        Scale Scale = Scale::stretch,
        Linearity  Linearity = Linearity::Dots,
        double CellAspectRatio = 0.5
        );
    ~TextChart();
    
};
