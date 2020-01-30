#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include "lexer.h"
#include "syntax.h"



using namespace std;

int main()
{
    Lexer lex;
    Syntax syn;
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
    //lex.print();
    // create a function that handles errors before trying to compile it
    syn.parser(lex.get_tokens(), lex.get_lexems());

    return 0;
}