Sequence to Graph
=================

How to use
----------

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


