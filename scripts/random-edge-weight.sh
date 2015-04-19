awk 'BEGIN { srand() }; {print $0" "int(0xFFFF * rand())}' $@
