# test 1: case 9, the hex input is wrong; case 10, try to test the arbitrary size, if the code can handle arbitary, it should output correct result, otherwise it should output some error information.
echo " "
echo "test calc"
./calc - b11111111 o55 d
./calc - o77 -d10 x
./calc + d999 xabcdef b
./calc - xae o7007 d
./calc + b1000000000000001 d1111 o
./calc - o1234567 x10F d
./calc - x15 -d10000 o
./calc - d10 -d4 b
./calc - xfx b1101 d
./calc - xFF o360 d
echo " "

echo "test calc, very long integer"
./calc + xFFFFFFFFFFFF d1 d
./calc + x7BCDEFABCDEFABCFF d1 d
./calc + d11111111111111111111111111111111111111111111111111111111111000 d33333333333333333333333333333333333333333333333333333333333789 d
./calc - d11111111111111111111111111111111111111111111111111111111111000 d33333333333333333333333333333333333333333333333333333333333789 d
echo " "

echo "test calc, multiplication"
./calc "*" xf o24 d
./calc "*" -d10 -d16 x
./calc "*" x3039 b11110001001000000 o
./calc "*" b1011 o143 x
echo " "
# test 2: case 3 has error, it includes a letter "l"; case 10 has error, beyond 32 bits
echo "test format"
./format 00000111010110111100110100010101 int
./format 10000001010000100100001101000100 int
./format 01111l11100000000000000000000001 int
./format 00111101100000000000000000000000 float
./format 11000000000101001100110011001100 float
./format 10000000000000000000000000000000 float
./format 01111111100000000000000000000000 float
./format 11111111100000000000000000000000 float
./format 01111111100000000000000000000001 float
./format 110000001011010000000000000000001 float
echo " "
