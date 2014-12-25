export SCRIPT_DIR=$( cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd )
$SCRIPT_DIR/../Release/generator.exe $@ | awk '{if ($1!=$2) print $0}' | sort -n -k 1 -k 2 | uniq -u
