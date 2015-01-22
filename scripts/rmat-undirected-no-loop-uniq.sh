export SCRIPT_DIR=$( cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd )
$SCRIPT_DIR/../Release/generator.exe $@ -n -u | sort -n -u -k 1 -k 2
