CC=g++
CC_FLAGS=-g -c

Debug: main.cpp chartLib.a
	@echo "Kompilacja kodu źródłowego z wykorzystaniem $(CC)..."
	@$(CC) -g main.cpp chartLib.a -o Main
	@echo "Done."

static: chartLib.a clean
	@echo "Generowanie biblioteki staticznej..."

chartLib.a:  Plot.o PlotData.o PlotDataSet.o
	@echo "Generowanie archiwum..."
	@ar rs chartLib.a Plot.o PlotData.o PlotDataSet.o

%.o: ./Source/%.cpp ./Header/%.hpp
	@echo "Kompilowanie pliku $< do $@... ( $(CC) $(CC_FLAGS) )" 
	@$(CC) $(CC_FLAGS) -o $@ $<

clean:
	@echo "Czyszczenie wszystkich plików obiektowych, main, oraz Main..."
	@rm -rf *.o  main Main

clean-all:
	@echo "Czyszczenie wszystkich plików obiektowych, archiwalnych, main, oraz Main..."
	@rm -rf *.o *.a main Main
