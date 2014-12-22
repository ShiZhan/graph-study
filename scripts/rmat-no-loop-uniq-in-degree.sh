export SCRIPT_DIR=$( cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd )
$SCRIPT_DIR/rmat-no-loop-uniq.sh $@ | awk '{print $2}' | sort -n | uniq -c | sort -n -r -k 1
