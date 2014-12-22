export SCRIPT_DIR=$( cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd )
gunzip -c $1 | sed '/BeginHeader/,/EndHeader/d;/^\s\+Disk[Read|Write]/!d' | awk 'BEGIN {FS=","} {print $6}' | $SCRIPT_DIR/../Debug/sequence-to-graph.exe $2 $3 $4 $5 | sort -n | uniq -c | wc -l
