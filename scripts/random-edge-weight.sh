awk 'BEGIN { srand() }; {print $0" "int(ARGV[2] * rand())}' $@
