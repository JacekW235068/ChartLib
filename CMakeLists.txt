cmake_minimum_required(VERSION 3.0.0)
project(ChartLib VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (SOURCES 
    Source/Plot.cpp;
    Source/PlotData.cpp;
    Source/PlotDataFunction.cpp;
    Source/PlotDataOrderedSet.cpp;
    Source/PlotDataSet.cpp;
)
add_library(ChartLib ${SOURCES})
target_include_directories(ChartLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)