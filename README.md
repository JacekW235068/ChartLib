# ChartLib

C++ library for drawing charts in terminal window

## Getting started
Makefile is still work in progress, the easiest way to start using it is:
1. use ```make static```, this command will create static library, there is master include file in project but keep in mind that it works in current file structure. Once you move it, you'll need to adapt this file.* 
2. Build a time machine and go forward to the moment in time where I've created package that builds it as shared lib.

*I will try to provide an simple include file in the future so it will work out of the box after compiling. 
## Features 
Currently there is one type of chart available
* Plot - basic type that draws (X,Y) pair dataset on the screen. Should be able to draw anything that implements PlotData Static class.

Charts created by this library are static and exist in program only as an array of chars. changing visible range or any other option results in need of redrawing everthing. 
