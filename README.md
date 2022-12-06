# boost_spsc_lockfree example

_to complile:_

g++ -Wall -std=c++14 -pthread main.cpp -o main



_input_:

./main 1 -x -8 98 56 32 f 4 5 -89 



_output:_

( 1 NaN -8) 

( 98 56 32) 

( NaN 4 5) 

( -89 NaN NaN) 

