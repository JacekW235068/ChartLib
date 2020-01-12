Main: main.o
	g++ -g main.o -o Main

main.o: main.cpp
	g++ -g -c main.cpp