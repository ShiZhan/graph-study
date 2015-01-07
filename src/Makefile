CC = g++

CXXFLAGS = --std=c++11 -O3

all:
	make generators translators

generators:
	make generator edge-sort seq-to-csr

translators:
	make seq-to-edges

generator: generator.cpp
	$(CC) generator.cpp $(CXXFLAGS) -o generator

edge-sort: edge-sort.cpp
	$(CC) edge-sort.cpp $(CXXFLAGS) -o edge-sort

seq-to-csr: seq-to-csr.cpp
	$(CC) seq-to-csr.cpp $(CXXFLAGS) -o seq-to-csr

seq-to-edges: seq-to-edges.cpp
	$(CC) seq-to-edges.cpp $(CXXFLAGS) -o seq-to-edges

clean:
	rm *.exe *.o generator edge-sort seq-to-csr seq-to-edges
