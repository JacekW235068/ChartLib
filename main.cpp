#include <iostream>
#include <stdlib.h>
#include "./Headers/TextChart.hpp"




int main(){
    std::pair<unsigned, unsigned> WindowSize = {10,10};
    char Symbol = 'x';
    std::vector<std::pair<double, double>> dataSet{{0,0},{1,1},{2,2},{3,3},{4,4},};
    bool Fast = false;
    TextChart t(WindowSize,Symbol, dataSet, Fast);
    t.createPrintableData();
    t.Draw(std::cout);
    return 0;
}