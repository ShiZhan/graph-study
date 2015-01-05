Graph Study Codes
=================

Zhan.Shi @ 2014

sequence to graph
-----------------

Transform a number sequence to an edge list.

1. read sequence, transform to edge list (diffuse=8, aliment=1M), output to console.

(Linux/UNIX Shell) cat sequence | sequence-to-graph 8 20

(Windows Console) type sequence | sequence-to-graph 8 20

sequence-to-graph 8 20 < sequence

2. read sequence start from line 10, total 10000 lines, transform to edge list and sort the output

cat sequence | tail -n+10 | head -10000 | sequence-to-graph 8 20 | sort -n

3. read sample trace, pick DiskRead/DiskWrite lines, select LBA column and transform into edge list.

sed -e '/BeginHeader/,/EndHeader/d' data-sample/Exchange-Server-Traces-sample.csv | grep "Disk[Read|Write]" | awk -F',' '{print $6}' | sequence-to-graph.exe 8 20

4. read trace.csv from gzipped trace.csv.gz, put output to trace.csv.edges

gunzip -c trace.csv.gz | sed -e '/BeginHeader/,/EndHeader/d' | grep "Disk[Read|Write]" | awk -F',' '{print $6}' | sequence-to-graph.exe 8 20 > trace.csv.edges

generator
---------

[R-MAT](http://www.cs.cmu.edu/~christos/PUBLICATIONS/siam04.pdf) graph Generator, output to console as random edge list.

The edges are generated online, without (possibly huge) in-memory adjacency list/matrix.

edge sort
---------

Sort edge list from console, remove duplicates, then output to console.

Temporary in-memory sort list can be spilled to file for processing huge incoming edges.

Also shell utilities such as sort and awk can do the job.

sequential edges to CSR
-----------------------

Transform sequential edges to [Compress Sparse Row](http://en.wikipedia.org/wiki/Sparse_matrix) file (row index and column index).

