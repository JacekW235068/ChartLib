CXXFLAGS=
OBJFILES= PlotData.o Plot.o PlotDataSet.o 

all:	compile
 
debug: CXXFLAGS+= -DDEBUG -g
debug: compile

compile: main.cpp chartLib.a
	@echo "Compiling using $(CXX)..."
	@$(CXX) $(CXXFLAGS) main.cpp chartLib.a -o Main
	@echo "Done."

static: chartLib.a
	@echo "Done."

chartLib.a: $(OBJFILES)
	@echo "Archiving files: $(OBJFILES)"
	@ar vrs chartLib.a obj/*.o


%.o: ./Source/%.cpp ./Header/%.hpp
	@echo "Compiling file $< to obj/$@... ( $(CXX) $(CXXFLAGS) )" 
	@$(CXX) -c -o obj/$@ $<

clean-main:	
	@echo "Removing exec Main..."
	@rm -rf Main

clean-obj:  
	@echo "Removing all objective files..."
	@rm -rf obj/*.o

clean-all: clean-main clean-obj
	@echo "Removing dynamic library..."
	@rm -rf chartLib.a
