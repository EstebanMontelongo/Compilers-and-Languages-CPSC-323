#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include "lexer.h"

using namespace std;



int main()
{
    Lexer lex;
    bool fileRead = false;
    string fileName;
   
    // Ask the user for file name until he gives a valid input name
    while(!fileRead)
    {
        cout << "Please Input the file name of your source code. " << endl;
        cin >> fileName;
        cin.clear();

        fileRead = lex.readFile(fileName); 
    }

    // Print out the tokens and lexems
    lex.print();

    return 0;
}

