export SCRIPT_DIR=$( cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd )
$SCRIPT_DIR/../vs2012/Release/generator.exe $@ -n | sort -n -u -k 1 -k 2
