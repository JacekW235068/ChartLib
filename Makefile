Main: main.cpp Plot.o PlotData.o PlotDataSet.o
	g++ -g main.cpp Plot.o PlotData.o PlotDataSet.o -o Main


Plot.o: ./Source/Plot.cpp
	g++ -g -c ./Source/Plot.cpp  -o Plot.o

PlotData.o: ./Source/PlotData.cpp
	g++ -g -c ./Source/PlotData.cpp  -o PlotData.o

PlotDataSet.o: ./Source/PlotDataSet.cpp
	g++ -g -c ./Source/PlotDataSet.cpp  -o PlotDataSet.o