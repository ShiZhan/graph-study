CC = g++

CXXFLAGS = --std=c++11 -O3

all:
	make generators translators

generators:
	make generator edge-sort sequential-edges-to-csr

translators:
	make sequence-to-graph

generator: generator.cpp
	$(CC) generator.cpp $(CXXFLAGS) -o generator

edge-sort: edge-sort.cpp
	$(CC) edge-sort.cpp $(CXXFLAGS) -o edge-sort

sequential-edges-to-csr: sequential-edges-to-csr.cpp
	$(CC) sequential-edges-to-csr.cpp $(CXXFLAGS) -o sequential-edges-to-csr

sequence-to-graph: sequence-to-graph.cpp
	$(CC) sequence-to-graph.cpp $(CXXFLAGS) -o sequence-to-graph

clean:
	rm *.exe *.o generator edge-sort sequential-edges-to-csr sequence-to-graph
