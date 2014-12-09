gunzip -c $1 | sed -e '/BeginHeader/,/EndHeader/d' | awk 'BEGIN {FS=","} /^\ +Disk(Read|Write),/{print $6}' | Debug/sequence-to-graph.exe $2 $3 | sort -n | uniq -c | wc -l
