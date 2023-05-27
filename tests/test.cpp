#include <ChartLib/Plot.hpp>
#include <ChartLib/PlotDataSet.hpp>
#include <ChartLib/IDecoration.hpp>

#include <catch2/catch_test_macros.hpp>

#include <cctype>

// TODO: CTest?
TEST_CASE("Interaction with PlotData object" "[Plot]")
{
    chart::Plot sut({10,10});
    sut.setVisibleRange({-2.0,2.0},{-1.5,1.5});
    std::list<std::pair<double, double>> data = {{-1.1,0.9}};
    std::shared_ptr<chart::PlotDataSet> dataSet(new chart::PlotDataSet(data,'o'));

    SECTION("No data")
    {
        std::string out = sut.print();
        REQUIRE(std::all_of(out.begin(),out.end(),[](const char c) { return std::isspace(c);}));    
    }
    SECTION("Single dataset")
    {
        sut.addDataSet(dataSet);
        std::string out = sut.print();
        REQUIRE(std::count(out.begin(), out.end(), 'o') == 1);
    }
    SECTION("Removed dataset")
    {
        sut.addDataSet(dataSet);
        sut.removeDataSet(dataSet);
        std::string out = sut.print();
        REQUIRE(std::count(out.begin(), out.end(), 'o') == 0);
    }
    SECTION("Single dataset added twice")
    {
        sut.addDataSet(dataSet);
        sut.addDataSet(dataSet);
        std::string out = sut.print();
        REQUIRE(std::count(out.begin(), out.end(), 'o') == 1);
        sut.removeDataSet(dataSet); // Single remove because second add was ignored
        out = sut.print();
        REQUIRE(std::count(out.begin(), out.end(), 'o') == 0);

    }
    SECTION("Dataset out of scope")
    {
        std::string out;
        {
            std::shared_ptr<chart::PlotDataSet> localDataSet(new chart::PlotDataSet(data,'o'));
            sut.addDataSet(localDataSet);
            out = sut.print();
        }
        REQUIRE(std::count(out.begin(), out.end(), 'o') == 1);
        out = sut.print();
        REQUIRE(std::count(out.begin(), out.end(), 'o') == 0);

    }
}

class TestingDecoration : public chart::IDecoration
{
    virtual std::tuple<int,int,int,int> drawFrame(
        std::map<std::pair<int,int>, std::string>& ChartMap,
        const std::pair<uint16_t,uint16_t>& WindowSize,
        const std::tuple<double,double,double,double>& VisibleRange)
    {
        return {-1,WindowSize.first,0,WindowSize.second+1};
    }
    
    virtual bool isForced() const
    {
        return true;
    };
    virtual ~TestingDecoration() = default;
};

TEST_CASE( "Drawing has proper size", "[Drawing]")
{
    const int x=10 ,y=10;
    chart::Plot sut({x,y});
    chart::IDecoration* decoration = new TestingDecoration();

    SECTION("Empty plot")
    {
        std::string out = sut.print();
        // + y because of \ns 
        REQUIRE(out.size() == x*y + y);
    }
    SECTION("Decoration expanding size")
    {
        sut.addDecoration(decoration);
        std::string out = sut.print();
        std::cout << "---\n" <<  out << "---\n";
        REQUIRE(out.size() == (x+1)*(y+1) + y+1);
    }

    // TODO: get rid of row pointers on interface
    delete decoration;
}