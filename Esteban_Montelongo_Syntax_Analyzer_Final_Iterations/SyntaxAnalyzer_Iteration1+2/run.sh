#/bin/bash

# Shell script to compile source code and link object code to create a .out

rm *.o
rm *.out

echo "Compiling main.cpp..."
g++ -c -m64 -Wall -fno-pie -no-pie -o main.o main.cpp

echo "Compiling lexer.cpp..."
g++ -c -m64 -Wall -fno-pie -no-pie -o lexer.o lexer.cpp

echo "Compiling syntax.cpp..."
g++ -c -m64 -Wall -fno-pie -no-pie -o syntax.o syntax.cpp

echo "Link the object files..."
g++ -m64 -fno-pie -no-pie -o SyntaxAnalyzer.out lexer.o syntax.o main.o 

echo "Executing Lexical Analyzer..."
echo " "
./SyntaxAnalyzer.out


