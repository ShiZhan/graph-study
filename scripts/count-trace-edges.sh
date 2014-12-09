gunzip -c $1 | sed -e '/BeginHeader/,/EndHeader/d' | grep "Disk[Read|Write]" | awk -F',' '{print $6}' | Debug/sequence-to-graph.exe $2 $3 | sort -n | uniq -c | wc -l
