awk 'BEGIN { srand()} {print $0" "int(65535 * rand())}' $@ | sort -n -k3 | awk '{print $1" "$2}'
