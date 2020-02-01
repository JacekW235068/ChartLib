Main: main.cpp Plot.o
	g++ -g main.cpp Plot.o -o Main


Plot.o: ./Source/Plot.cpp
	g++ -g -c ./Source/Plot.cpp  -o Plot.o