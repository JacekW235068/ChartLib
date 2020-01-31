#pragma once

#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

#include "../Enum/Scale.cpp"
#include "../Enum/Style.cpp"


class TextChart
{
//protected:
public:
    //DATA
    //chart settings
    Style style;
    Scale scale;
    //terminal character cell aspect ratio X/Y
    double cellAspectRatio;
    //Array of printable data
    char** printableData;
    std::pair<unsigned, unsigned> windowSize;
    //symbol used for drawing chart
    char symbol;
    //range of chart
    double min_y;
    double min_x;
    double max_y;
    double max_x;
    double visible_min_y;
    double visible_min_x;
    double visible_max_y;
    double visible_max_x;

    //CONSTRUCTORS
    TextChart(std::pair<unsigned, unsigned> WindowSize,
        std::vector<std::pair<double, double>> &DataSet,
        char Symbol = 'o',
        Scale Scale = Scale::stretch,
        Style Style =Style::dots,
        double CellAspectRatio = 0.5
        );
    ~TextChart();

    //METHODS
    //find min/max X/Y
    void setRange(std::vector<std::pair<double, double>>& DataSet);
    char** createChart(std::vector<std::pair<double, double>>& DataSet);
    //scaling methods, sets visible range
    std::pair<double, double> valueRange_scalex();
    std::pair<double, double> valueRange_scaley();
    std::pair<double, double> valueRange_stretch();
    //Style methods, "draws" symbol onto printable data
    void drawDots(std::vector<std::pair<double, double>>& DataSet, std::pair<double, double>& range);
    void drawLines(std::vector<std::pair<double, double>>& DataSet, std::pair<double, double>& range);
    //Draw line between p1 and p2, no safety pass correct args
    void drawLine(std::pair<int, int> p1, std::pair<int, int> p2);


    
    //FRIENDS AND STUFF
    friend std::ostream& operator<<(std::ostream& s, const TextChart& t);
};
