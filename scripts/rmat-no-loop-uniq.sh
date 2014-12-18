Debug/generator.exe $1 $2 $3 | awk '{if ($1!=$2) print $0}' | sort -n | uniq -u
