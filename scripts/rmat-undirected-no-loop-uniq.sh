export SCRIPT_DIR=$( cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd )
$SCRIPT_DIR/../Debug/generator.exe $@ | awk '{if ($1!=$2) print $1" "$2"\n"$2" "$1}' | sort -n | uniq -u