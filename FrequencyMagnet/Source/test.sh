clang++ PitchInfoParser.cpp testParser.cpp -std=c++11 -o test.bin $@
./test.bin
rm test.bin
