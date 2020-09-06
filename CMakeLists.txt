cmake_minimum_required(VERSION 3.0.0)
project(ChartLib VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Plot.cpp;
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/PlotData.cpp;
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/PlotDataFunction.cpp;
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/PlotDataSet.cpp;
)
add_library(ChartLib ${SOURCES})
target_include_directories(ChartLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)