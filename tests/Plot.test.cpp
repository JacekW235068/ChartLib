#include <ChartLib/Plot.hpp>
#include <ChartLib/PlotDataSet.hpp>
#include <ChartLib/IDecoration.hpp>

#include <catch2/generators/catch_generators.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cctype>

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
    virtual void drawFrame(
        std::map<std::pair<int,int>, std::string>& ChartMap,
        const std::pair<uint16_t,uint16_t>& WindowSize,
        const std::tuple<double,double,double,double>&)
    {
        ChartMap[{-1,5}] = " ";
        // This is actually out of window size, Indexing !!!
        ChartMap[{0,WindowSize.second}] = " ";
    }
    
    virtual bool isForced() const
    {
        return true;
    };
    virtual ~TestingDecoration() = default;
};

TEST_CASE( "Drawing has proper size", "[Drawing]")
{
    const int x=10 ,y=12;
    chart::Plot sut({x,y});
    sut.setVisibleRange({0.0,1.0},{0.0,1.0});
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
        REQUIRE(out.size() == (x+1)*(y+1) + y+1);
    }

    // TODO: get rid of row pointers on interface
    delete decoration;
}

TEST_CASE( "Puting data on the plot", "[Drawing]")
{
    const int x=10 ,y=12;
    chart::Plot sut({x,y});
    sut.setVisibleRange({-1.0,1.5},{2.0,2.5});

    SECTION ("only visible dots are on the plot")
    {
        std::list<std::pair<double, double>> data = {{-1.0,0.0},{-0.9,2.2},{-0.0,2.3}};
        std::shared_ptr<chart::PlotDataSet> dataSet(new chart::PlotDataSet(data,'o',"test set",chart::Color::none,chart::Style::Dots));
        sut.addDataSet(dataSet);
        auto out = sut.print();
        std::cout << "----------\n" << out;
        REQUIRE(std::count(out.begin(),out.end(),'o') == 2);
    }
    SECTION ("Dots at borders are visible")
    {
        std::list<std::pair<double, double>> data = {
            {-1.0,2.0},
            {-1.0,2.5},
            { 1.5,2.0},
            { 1.5,2.5},
            {-1.0,2.2},
            { 0.0,2.5}
        };
        std::shared_ptr<chart::PlotDataSet> dataSet(new chart::PlotDataSet(data,'o',"test set",chart::Color::none,chart::Style::Dots));
        sut.addDataSet(dataSet);
        auto out = sut.print();
        std::cout << "----------\n" << out;
        REQUIRE(std::count(out.begin(),out.end(),'o') == 6);
    }
    SECTION ("Full line on the plot")
    {
        std::list<std::pair<double, double>> data = GENERATE(
            std::list<std::pair<double, double>>{{-0.9,2.2},{0.0,2.3}}, // Full line on the plot
            std::list<std::pair<double, double>>{{-2.0,2.2},{0.0,2.3}}, // Half line on the plot
            std::list<std::pair<double, double>>{{-2.0,2.2},{3.0,2.3}}, // Line with no ends on the plot
            std::list<std::pair<double, double>>{{-1.5,1.5},{2.0,3.0}}, // Line with no ends in any visible boundries
            std::list<std::pair<double, double>>{{2.0,3.0},{-1.5,1.5}}  // Reversed Points
        );
        std::shared_ptr<chart::PlotDataSet> dataSet(new chart::PlotDataSet(data,'o',"test set",chart::Color::none,chart::Style::Linear));
        sut.addDataSet(dataSet);
        auto out = sut.print();
        std::cout << "----------\n" << out;
        REQUIRE(std::count(out.begin(),out.end(),'o') > 2);
    }
}