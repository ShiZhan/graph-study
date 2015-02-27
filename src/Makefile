include Makefile.config

CC = g++

CXXFLAGS = --std=c++11 -O3

USE_BOOST = -I$(BOOST_DIR)

all:
	make generators algorithms bgl-shell v-map

generators:
	make generator edge-sort seq-to-csr seq-to-edges

algorithms:
	make sssp bfs dfs

generator: generator.cpp
	$(CC) generator.cpp $(CXXFLAGS) -o generator

edge-sort: edge-sort.cpp
	$(CC) edge-sort.cpp $(CXXFLAGS) -o edge-sort

seq-to-csr: seq-to-csr.cpp
	$(CC) seq-to-csr.cpp $(CXXFLAGS) -o seq-to-csr

seq-to-edges: seq-to-edges.cpp
	$(CC) seq-to-edges.cpp $(CXXFLAGS) -o seq-to-edges

sssp: sssp.cpp
	$(CC) sssp.cpp $(CXXFLAGS) -o sssp

bfs: bfs.cpp
	$(CC) bfs.cpp $(CXXFLAGS) -o bfs

dfs: dfs.cpp
	$(CC) dfs.cpp $(CXXFLAGS) -o dfs

bgl-shell: bgl-shell.cpp
	$(CC) bgl-shell.cpp $(CXXFLAGS) $(USE_BOOST) -o bgl-shell

v-map: v-map.cpp
	$(CC) v-map.cpp $(CXXFLAGS) -o v-map

clean:
	rm *.exe *.o generator edge-sort seq-to-csr seq-to-edges sssp bfs dfs bgl-shell v-map
