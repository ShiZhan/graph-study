SNIA Trace to Graph
===================

How to use
----------

1. read trace.csv, transform to edge list (diffuse=8, block=1MB), output to console.

(Linux/UNIX Shell) cat trace.csv | generator 8 20

(Windows Console) type trace.csv | generator 8 20

generator 8 20 < trace.csv

2. read trace.csv start from line 10, total 10000 lines, transform to edge list and sort the output

cat trace.csv | tail -n+10 | head -10000 | generator 8 20 | sort -n

3. read trace.csv from gzipped trace.csv.gz, put output to trace.csv.edges

gunzip -c trace.csv.gz | generator 8 20 > trace.csv.edges


