cmake_minimum_required(VERSION 3.0.0)
project(ChartLib VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-pedantic -Wall -Wextra -Wno-range-loop-construct)
# TODO: CTest
set (SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Plot.cpp;
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/PlotData.cpp;
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/PlotDataFunction.cpp;
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/PlotDataSet.cpp;
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/SimpleDoubleLineFrame.cpp;
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/XAxisLabels.cpp;
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/YAxisLabels.cpp;
)
add_library(ChartLib ${SOURCES})
target_include_directories(ChartLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)


# TODO: add come sort of configuration so that tests are not build in normal usage
find_package(Catch2 3 REQUIRED)
# These tests can use the Catch2-provided main
add_executable(tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/Plot.test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE ChartLib)