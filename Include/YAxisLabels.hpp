#include <IDecoration.hpp>


namespace chart{
class YAxisLabels : public IDecoration{
private:
    int precision;
    double jump;
    double start;
public:
    YAxisLabels(uint Precision, double Jump = -1.0, double Start = 0.0);
    virtual ~YAxisLabels() = default;
    std::tuple<int,int,int,int> drawFrame(
        std::map<std::pair<int,int>, std::string>& ChartMap,
        const std::pair<uint16_t,uint16_t>& WindowSize,
        const std::tuple<double,double,double,double>& VisibleRange) override;
    bool isForced() const override;
    void setPrecision(int Precision);
    int getPrecision();
    void setJump(double Jump);
    double getJump();
    void setStar(double Start);
    double getStar();
};
}