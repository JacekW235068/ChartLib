
CXXFLAGS=-std=c++1z 


OBJFILES= PlotData.o Plot.o PlotDataSet.o PlotDataOrderedSet.o PlotDataFunction.o


all:	static
allWall: CXXFLAGS+= -Wall -Wpedantic -Wextra
allWall: static

.PHONY: clean-main clean-obj create_dir clean_all
.IGNORE: create_dir 

debugWall: CXXFLAGS+= -DDEBUG -g -Wall -Wpedantic -Wextra
debugWall: exec
debug: CXXFLAGS+= -DDEBUG -g
debug: exec

exec: 	main.cpp chartLib.a
	@echo "Compiling using $(CXX)..."
	@$(CXX) $(CXXFLAGS) main.cpp chartLib.a -o Main
	@echo "Done."


static: chartLib.a
	@echo "Done."

chartLib.a: create_dir $(OBJFILES)
	@echo "Archiving files: $(OBJFILES)"
	@ar vrs chartLib.a obj/*.o


create_dir: 
	@mkdir -vp obj
  
%.o: ./Source/%.cpp ./Header/%.hpp
	@echo "Compiling file $< to obj/$@... ( $(CXX) $(CXXFLAGS) )" 
	@$(CXX) $(CXXFLAGS) -c -o obj/$@ $<

clean-main:	
	@echo "Removing exec Main..."
	@rm -rf Main


clean-obj:  
	@echo "Removing all objective files..."
	@rm -rf obj/*.o

clean-all: clean-main clean-obj
	@echo "Removing dynamic library..."
	@rm -rf chartLib.a
