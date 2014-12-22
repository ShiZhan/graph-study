export SCRIPT_DIR=$( cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd )
$SCRIPT_DIR/../Debug/generator.exe $@ | awk '{if ($1!=$2) print $0}' | sort -n | uniq -u
