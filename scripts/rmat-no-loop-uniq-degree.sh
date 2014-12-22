export SCRIPT_DIR=$( cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd )
$SCRIPT_DIR/rmat-no-loop-uniq.sh $@ | awk '{print $1}' | uniq -c | sort -n -r -k 1
