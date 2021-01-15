#!/bin/bash
RED='\033[0;31m'
GRN='\033[0;32m'
NC='\033[0m'

make testpairings_cifer 1>/dev/null
./testpairings_cifer >tmp
diff tmp output_test 1>/dev/null
if [ $? == 0 ]; then
	printf "PAIRINGS TEST with CiFer+acml libs ${GRN}OK${NC}\n"
	rm tmp
else
	printf "PAIRINGS TEST with CiFer+acml libs ${RED}FAILED${NC}\n"
fi
make testpairings_pbc 1>/dev/null
./testpairings_pbc >tmp
diff tmp output_test 1>/dev/null
if [ $? == 0 ]; then
	printf "PAIRINGS TEST with pbc lib ${GRN}OK${NC}\n"
	rm tmp
	else
	printf "PAIRINGS TEST with pbc lib ${RED}FAILED${NC}\n"
	rm tmp
fi
