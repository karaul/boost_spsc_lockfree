# boost_spsc_lockfree example

complile:
g++ -std=c++14 -pthread main.cpp -o main

input:
./main 1 -x -8 98 56 32 f 4 5 -89 

output:
( 1 NaN -8) 
( 98 56 32) 
( NaN 4 5) 
( -89 NaN NaN) 
