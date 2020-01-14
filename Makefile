Main: main.cpp TextChart.o
	g++ -g main.cpp TextChart.o -o Main


TextChart.o: ./Source/TextChart.cpp
	g++ -g -c ./Source/TextChart.cpp -o TextChart.o