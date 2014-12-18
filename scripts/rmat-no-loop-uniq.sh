Debug/generator.exe $@ | awk '{if ($1!=$2) print $0}' | sort -n | uniq -u
