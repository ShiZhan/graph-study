gunzip -c $1 | sed '/BeginHeader/,/EndHeader/d;/^\s\+Disk[Read|Write]/!d' | awk 'BEGIN {FS=","} {print $6}' | Debug/sequence-to-graph.exe $2 $3 $4 $5 | awk '{if ($1!=$2) print $0}' | sort -n | uniq -c | sort -n -r -k 1 -t ' ' | less
