gunzip -c $1 | sed '/BeginHeader/,/EndHeader/d;/^\s\+Disk[Read|Write]/!d' | awk 'BEGIN {FS=","} {print $6}' | Debug/sequence-to-graph.exe $2 $3 $4 $5 | sort -n | uniq -c | wc -l