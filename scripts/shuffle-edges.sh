awk 'BEGIN { srand() }; {print int(65535 * rand())"^"$0}' $@ | sort -n -t '^' -k 1 | awk 'BEGIN { FS = "^" }; {print $2}'
