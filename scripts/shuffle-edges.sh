awk 'BEGIN { srand() }; {print int(0xFFFFFFFF * rand())"^"$0}' $@ | sort -n -t '^' -k 1 | awk 'BEGIN { FS = "^" }; {print $2}'
