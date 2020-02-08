CC=g++


Debug: main.cpp chartLib.a
	$(CC) -g main.cpp chartLib.a -o Main

static: chartLib.a clean

chartLib.a:  Plot.o PlotData.o PlotDataSet.o
	ar rs chartLib.a Plot.o PlotData.o PlotDataSet.o

%.o: ./Source/%.cpp ./Header/%.hpp
	$(CC) -g -c -o $@ $<

clean:
	rm -rf *.o  main Main

clean-all:
	rm -rf *.o *.a main Main