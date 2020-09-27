#include <IDecoration.hpp>


namespace chart{
class XAxisLabels : public IDecoration{
private:
    int precision;
    double jump;
public:
    XAxisLabels(uint Precision, double Jump = -1.0);
    virtual ~XAxisLabels() = default;
    std::tuple<int,int,int,int> drawFrame(
        std::map<std::pair<int,int>, std::string>& ChartMap,
        const std::pair<uint16_t,uint16_t>& WindowSize,
        const std::tuple<double,double,double,double>& VisibleRange) override;
    bool isForced() const override;
    void setPrecision(int Precision);
    int getPrecision();
};
}