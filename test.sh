#!/bin/bash
for (( i=1; i<=6; ++i)) 
do
    for (( j=1; j<=6; ++j))
    do
        if [ $i != 3 ] || [ $j == 1 ]; then 
            a=$(($i*100 + $j))
            g++ -std=c++20 -Wall -Wextra -O2 -DTEST_NUM=$a organism_test.cc -o organism
            ./organism
            echo "$(tput bold)$(tput setaf 1)NUMER TESTU: $a"
        fi
    done
done
for (( i=2; i<=4; ++i)) 
do 
    a=$((300 + $i))
    g++ -std=c++20 -Wall -Wextra -O2 -DTEST_NUM=$a organism_test.cc -o organism
    ./organism

    echo "$(tput bold)$(tput setaf 1)MA SIE NIE KOMPILOWAC :) NUMER TESTU: $a"
done
