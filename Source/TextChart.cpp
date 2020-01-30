#include "../Header/TextChart.hpp"



TextChart::TextChart(std::pair<unsigned, unsigned> WindowSize,
        std::vector<std::pair<double, double>> &DataSet,
        char Symbol,
        Scale Scale,
        Linearity  Linearity,
        double CellAspectRatio
        ) : windowSize(WindowSize),
        symbol(Symbol),
        linearity(Linearity),
        scale(Scale),
        cellAspectRatio(CellAspectRatio)

{
    printableData = new char*[windowSize.second];
    for(int i = 0; i < windowSize.second; i++){
        printableData[i] = new char[windowSize.first];   
    }
    createPrintableData(DataSet);
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

char** TextChart::createPrintableData(std::vector<std::pair<double, double>>& dataSet){
    setRange(dataSet);
    std::pair<double,double> range;
    //calculate value range, set boundries
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
    //draw chart
    switch (linearity)
    {
    case Linearity::Linear:
        drawLines(dataSet, range);
        break;
    default:
        drawDots(dataSet, range);
        break;
    }
    return printableData;
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
    bool previous = false;
    //prefill printable array of chars
    for(int i = 0; i < windowSize.second; i++){
        for(int j = 0; j < windowSize.first; j++)
            printableData[i][j] = ' ';
    }
    std::pair<int,int> previousCoords(
        static_cast<int>(round((DataSet.begin()->first-visible_min_x)/range.first*(windowSize.first-1))),
        static_cast<int>(round((DataSet.begin()->second-visible_min_y)/range.second*(windowSize.second-1)))
    );
    previous = (previousCoords.first >= 0 && previousCoords.second >= 0 &&
     previousCoords.first < windowSize.first && previousCoords.second < windowSize.second);
    std::for_each(++DataSet.begin(), DataSet.end(), [&](std::pair<double,double>& node){
        int y = static_cast<int>(round((node.second-visible_min_y)/range.second*(windowSize.second-1)));
        int x =static_cast<int>(round((node.first-visible_min_x)/range.first*(windowSize.first-1)));
        if(x >= 0 && y >= 0 && x < windowSize.first && y < windowSize.second && previous){
            if (x == previousCoords.first){
                for(int i = std::min(previousCoords.second, y); i <= std::max(previousCoords.second, y); i++)
                    printableData[i][x] = symbol;
            }
            else if (y == previousCoords.second){
                for(int i = std::min(previousCoords.first, x); i <= std::max(previousCoords.first, x); i++)
                    printableData[y][i] = symbol;
            }
            else{
                //ax+b=y
                double a = static_cast<double>(y - previousCoords.second) / (x - previousCoords.first);
                double b = node.second - (a*node.first);
                double jump;
                int min_value;
                int max_value;
                if(abs(x - previousCoords.first) > abs(y - previousCoords.second)){
                    min_value = std::min(previousCoords.first, x);
                    max_value = std::max(previousCoords.first, x);
                    for(int i = min_value; i <= max_value; i++){
                        printableData[static_cast<int>(a*i + b)][i];
                    } 
                }else{
                    min_value = std::min(previousCoords.second, y);
                    max_value = std::max(previousCoords.second, y);
                    for(int i = min_value; i <= max_value; i++){
                        printableData[i][static_cast<int>(static_cast<double>(i-b)/a)];
                    } 
                }
            }
            
              //TODO
        }
    });
}

//operators
std::ostream& operator<<(std::ostream& s, const TextChart& t){
    std::string output;
    output += "start\n"; 
    for(int i = t.windowSize.second-1; i >= 0; i--){
        for(int j = 0; j < t.windowSize.first; j++)
            output += t.printableData[i][j];
        output += '\n';
    }
    output += "end\n"; 
    s << output;
    return s;
}