#include <IDecoration.hpp>


namespace chart{
class SimpleDoubleLineFrame : IDecoration{
    std::tuple<int,int,int,int> drawFrame(
        std::map<std::pair<int,int>, std::string>& ChartMap,
        const std::pair<uint16_t,uint16_t>& WindowSize,
        const std::tuple<double,double,double,double>& VisibleRange) const override;
    bool isForced() const override;
};
}