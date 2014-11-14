CC = g++

BOOST = c:/mingw64/x86_64-w64-boost/

CXXFLAGS = --std=c++11 -I$(BOOST)include -L$(BOOST)lib -O3

all:
	make graph thread options

graph:
	make graph0 graph1 graph2 graph3 graph4 

graph0: graph0.cpp
	$(CC) graph0.cpp $(CXXFLAGS) -o graph0

graph1: graph1.cpp
	$(CC) graph1.cpp $(CXXFLAGS) -o graph1

graph2: graph2.cpp
	$(CC) graph2.cpp $(CXXFLAGS) -o graph2

graph3: graph3.cpp
	$(CC) graph3.cpp $(CXXFLAGS) -o graph3

graph4: graph4.cpp
	$(CC) graph4.cpp $(CXXFLAGS) -o graph4

thread: thread.cpp
	$(CC) thread.cpp $(CXXFLAGS) -o thread

options: options.cpp
	$(CC) options.cpp $(CXXFLAGS) -lboost_program_options-mgw48-mt-1_57 -o options

cclean: rm -f *.exe

