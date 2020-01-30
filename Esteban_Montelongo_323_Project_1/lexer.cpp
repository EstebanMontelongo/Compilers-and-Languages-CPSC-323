#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include "lexer.h"

using namespace std;

//================ Defining all keywords into hashtables ===============//
set<string> KEYWORDS = {"int", "float", "bool", "if", "else","then","endif",
                            "while", "whileend", "do", "doend", "for",
                            "forend", "input", "output","and", "or", "function"};
set<char> SEPARATORS = {'\'', '(', ')', '{', '}', '[', ']', ',', '.', ':', ';', ' ', '\n', '\t', '!', '\0'};
set<char> OPERATORS  = {'*', '+', '-', '=', '/', '>', '<', '%' };

// === state numeration list for switch statement in paser function ====//
enum state {Initial, Identifier, Whole, Real, Comment, IdentifierFinal, WholeFinal, RealFinal, SeperatorFinal, OperatorFinal, Unidentified, CommentFinal };

//======================================================================//
//	Description: Default contructor sets all values in the table to
//				 default values
//
//	Input: None
//	Output: None
//======================================================================//
Lexer::Lexer()
{
    currState_ = 0;
    currIndex_ = 0;
    currSize_ = sourceCode_.size();
    isComment_ = false;
}

//=======================================================================//
//    Description: This function will read a file, analyze and parse it.
//
//    Input: filename - String used to search for file
//    Output: Returns true if file read successfully, false otherwise
//======================================================================//
 bool Lexer::readFile(string filename)
{
    ifstream myFile(filename);
    string line;
    bool fileRead = false;

    if (myFile.is_open())
    {
        while(getline(myFile, line))
        { 
            line.append("\n");
            sourceCode_.append(line);
        }
        fileRead =  true;
        myFile.close();
    }
    else
    {
        cout << "Unable to open file" << endl;
        fileRead = false;
    }
    // If the file is read analyze it for parsing.
    if(fileRead) 
    {
        parser();
    }

    return fileRead;
}
//=======================================================================//
//	Description: Prints out all lexems with there tokens
//               
//	Input: none
//	Output:none
//======================================================================//
void Lexer::print()
 {
    //Prints out to a file 
    ofstream oFile;
    oFile.open("result.txt");
    oFile << endl;
    oFile << setw(5) << left << "TOKENS\t " <<"\tLEXEMS" << endl;
    oFile << endl;
    for(size_t i = 0; i < token_.size(); i++)
    {
        oFile <<setw(10) << left <<  token_[i] <<"\t" << lexems_[i] << endl;
    }

    //Prints out to the console
    cout << endl;
    cout << setw(5) << left << "TOKENS\t " <<"\tLEXEMS" << endl;
    cout << endl;
    for(size_t i = 0; i < token_.size(); i++)
    {
        cout <<setw(10) << left <<  token_[i] <<"\t" << lexems_[i] << endl;
    }
 }


//=======================================================================//
//    Description: This function will take in a character and convert it
//                 to its FSM column.
//
//    Input: c - Character in the source code.
//    Output: Returns the correct column; else -1337(randomly chosen value)
//======================================================================//
 int Lexer::checkInputs(char c)
 {
    if(toupper(c) >= 'A' && toupper(c) <= 'Z') 
        return 0;
    if(c >= '0' && c <= '9')
        return 1;
    if(c == '$')
        return 2;
    if(c == '.')
        return 3;
    if(isSeperator(c))
        return 4;
    if(isOperator(c))
        return 5;
    if(c == '!')
        return 6;

    return -1337;
 }
//=======================================================================//
//    Description: This function iterates throught the source code 
//                 parses it into its Tokens and Lexems
//
//    Input: none
//    Output: none
//======================================================================//
void Lexer::parser()
{
    string buffer;
    string s;
    int col = 0;
    int x = 0, y = 0;
    char c;
    char nextChar;


    // Determine the number of '!' in the string
    x = y = count(sourceCode_.begin(), sourceCode_.end(), '!');

    while(currIndex_ < sourceCode_.size())
    {
        
        c = sourceCode_[currIndex_];


        //Takes care is there is an odd number of '!' marks
        if(c == '!' && y > 1)
        {   
            isComment_ = !isComment_;
            y -= 1;
        }
        else if (c == '!' && y == 1)
        {
            if(x %2 == 0)
            {
                currState_ = Initial;
            }
            else
            {
               currState_ = SeperatorFinal; 
            }
            y -= 1;
            isComment_ = false;
        }
       
        
        // allows certain cases to view one character ahead if possible
        if (currIndex_ + 1 <= sourceCode_.size())
        {
            nextChar = sourceCode_[currIndex_+1];
        }
        col = checkInputs(c);
        if(col != -1337 && !isComment_ && c != '!') 
        {
            currState_ = stateTable_[currState_][col];
        }

        switch(currState_)
        {
            //===================== States ==============================//
            case Comment:
                buffer.clear();
                break;

            case Identifier:
                buffer.push_back(c);
                break;

            case Whole:
                buffer.push_back(c);
                break;

            case Real:
                buffer.push_back(c);
                break;
            //===================== Final Transition States==============================//
            case CommentFinal:
                buffer.clear();
                currState_ = Initial;
                break;

            case IdentifierFinal:
                if(isKeyWord(buffer) && !isSeperator(nextChar))
                {
                    token_.push_back("KeyWord");
                    lexems_.push_back(buffer);
                }
                else if(isKeyWord(buffer))
                {
                    token_.push_back("KeyWord");
                    lexems_.push_back(buffer);
                }
                else
                {
                    token_.push_back("Identifer");
                    lexems_.push_back(buffer);
                }
                
                buffer.clear();
                currIndex_ -= 1;
                currState_ = Initial;
                break;

            case WholeFinal:
                token_.push_back("Valid Integer");
                lexems_.push_back(buffer);
                buffer.clear();
                currIndex_ -= 1;
                currState_ = Initial;
                break;

            case RealFinal:
                token_.push_back("Valid Real");
                lexems_.push_back(buffer);
                buffer.clear();
                currIndex_ -= 1;
                currState_ = Initial;
                break;

            case SeperatorFinal:
                if(c != '\n' && !isblank(c) )
                {
                    token_.push_back("Seperators");
                    s.push_back(c);
                    lexems_.push_back(s);
                }
                s.clear();
                buffer.clear();
                currState_ = Initial;
                break;
        
            case OperatorFinal:
                token_.push_back("Operators");
                s.push_back(c);
                lexems_.push_back(s);
                s.clear();
                buffer.clear();
                currState_ = Initial;
                break;

            case Unidentified:
                token_.push_back("Udefined");
                s.append(buffer);
                s.push_back(c);
                lexems_.push_back(s);
                buffer.clear();
                currState_ = Initial;
                break;
        }
        currIndex_+=1;
    }
}
//=======================================================================//
//    Description: Tests wether the given input is in our 
//                 defined Operators list
//
//    Input: c - Character to test if it's an operator
//    Output: Returns true if operator, false otherwise
//======================================================================//
bool Lexer::isOperator(char c)
{
    bool retVal;

    if(OPERATORS.find(c) == OPERATORS.end())
    {
        retVal = false;
    }
    else
    {
        retVal =  true;
    }
    return retVal;
}
//=======================================================================//
//    Description: Tests wether the given input is in our 
//                 defined Operators list
//
//    Input: c - Character to test if it's an seperator
//    Output: Returns true if seperator, false otherwise
//======================================================================//
bool Lexer::isSeperator(char c)
{
    bool retVal;

    if(SEPARATORS.find(c) == SEPARATORS.end())
    {
        retVal = false;
    }
    else
    {
        retVal =  true;
    }
    return retVal;
}
//=======================================================================//
//    Description: Tests wether the given input is in our 
//                 defined keywords list
//
//    Input: c - Character to test if it's an operator
//    Output: Returns true if keyword, false otherwise
//======================================================================//
bool Lexer::isKeyWord(string test)
{
    bool retVal;

    if(KEYWORDS.find(test) == KEYWORDS.end())
    {
        retVal = false;
    }
    else
    {
        retVal =  true;
    }
    return retVal;
    
}