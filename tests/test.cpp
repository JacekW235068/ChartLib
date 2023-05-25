#include <ChartLib/Plot.hpp>
#include <ChartLib/PlotDataSet.hpp>

#include <catch2/catch_test_macros.hpp>

#include <cctype>

// TODO: CTest?
TEST_CASE("Interaction with PlotData" "[Plot]")
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

TEST_CASE("Interaction with PlotData" "[Legend]")
{
    
}