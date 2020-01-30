#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <stack>
#include "syntax.h"

using namespace std; 


//=======================================================================//
//    Description: This function uses a Predictive Paser Table to check if
//                 parsed tokens/lexems have the correct syntax.
//
//    Input: tokens - a vector of tokens (lexems share the same indecies)
//           lexems - a vector of lexems (tokens share the same indecies)
//    Output: none
//======================================================================//
void Syntax::parser(const vector<string> &tokens,const vector<string> &lexems)
{
    
    int column = -1;
    int currTop = -1;
    int derivation = -1;
    bool bool_print = true;
    ofstream oFile;
    oFile.open("syntax_result.txt");

    currStack_.push(ST);
    while(!currStack_.empty())
    {   

        currTop = currStack_.top();
        // If top of the stack is $ and end of the token list continue and loop should end.
        if(currTop == DS && currIndex_ > tokens.size()-1)
        {
            cout << "Finished!" << endl;
            oFile << "Finished!" << endl;
            currStack_.pop();
            continue;
        }
        // If all tokens have been read and the currTop of the stack is not $, error
        else if(currIndex_ >= tokens.size() && currTop != DS)
        {
            cout << "Error entire file has been read but stack is not empty!" << endl;
            oFile << "Error entire file has been read but stack is not empty!" << endl;
            oFile.close();
            return;
        }
        // Displays current token and lexem every new token
        if(bool_print && currIndex_ < tokens.size())
        {
            cout << "Token: \t " << tokens[currIndex_] <<  " Lexem: \t" <<  lexems[currIndex_] <<  endl;
            oFile << "Token: \t " << tokens[currIndex_] <<  " Lexem: \t" <<  lexems[currIndex_] <<  endl;
            bool_print = false;
        }
        
        // Top of the stack is equal to the current token (token must be a terminal)
        if(comp_token_to_stack(currTop, tokens[currIndex_], lexems[currIndex_]))
        {
            // The stack is not empty pop
            if(!currStack_.empty()) currStack_.pop();
            // Since I do not append $ to every statement ending in ; I push a (ST) to the stack to start another statement
            if(lexems[currIndex_][0] == ';' && currIndex_ != tokens.size()-1)
            {
                currStack_.push(ST);
            }
            // Allow the display of the next token/lexem
            bool_print = true;
            // Increment the token pointer
            currIndex_++;
        }
        else
        {
            // If the top of the stack is not equal token and the token is a terminal, error
            if(is_token_terminal(currTop))
            {
                cerr << "Error with terminal: " << term_to_string(currTop) << endl;
                oFile << "Error with terminal: " << term_to_string(currTop) << endl;
                oFile.close();
                return;
            }// If the top of stack not equal to token and its a non terminal display and pop
            else if(is_token_non_terminal(currTop))
            {
                // Display the left side production, if the function return a non empty string
                if(non_term_print_str(currTop) != "") 
                {
                    cout << non_term_print_str(currTop);
                    oFile << non_term_print_str(currTop);
                }
                column = token_to_column(tokens[currIndex_], lexems[currIndex_]);
                if(column != -1)
                {
                    derivation = LL_Table_[currTop][column];
                    if(derivation != -1)
                    {
                        if(!currStack_.empty()) currStack_.pop();
                        // Display the right side production, if the function does not return -1
                        cout << derivation_print_str(derivation);
                        oFile << derivation_print_str(derivation);
                        // Push the derivation value on the stack in reverse order
                        der_reverse_push(derivation);
                    }
                    else
                    {
                        cout << "Derivation DNE! \n";
                        oFile << "Derivation DNE! \n";
                        oFile.close();
                        return;
                    }
                }
                else
                {
                    cout << "\nError with Token: " << tokens[currIndex_] << "\t Lexem: " << lexems[currIndex_] << endl;
                    oFile << "\nError with Token: " << tokens[currIndex_] << "\t Lexem: " << lexems[currIndex_] << endl;
                    oFile.close();
                    return;
                }
                
            }
        }

    }
    if(currIndex_ != tokens.size())
    {
        cout << "Error, the stack is empty but the entire file has not been read" << endl;
        oFile << "Error, the stack is empty but the entire file has not been read" << endl;
        oFile.close();
        return;
    }
    oFile.close();

}


//=======================================================================//
//    Description: This is the default constructor for class Syntax
//
//    Input: none.
//    Output: none.
//======================================================================//
Syntax::Syntax()
{
    currStack_.push(DS);
    currIndex_ = 0;
}

//=======================================================================//
//    Description: This function will convert the predefined enum to a 
//                 string.
//
//    Input: symbol - predefined enum value {ST, AS, EX, EX_PRIME, TE, TE_PRIME, FA}
//    Output: Returns a string with the left side of a production rule.
//======================================================================//
string Syntax::non_term_print_str(int symbol)
{
    string retString = "";
    switch (symbol)
    {
    case ST:
        retString = "<Statement> -> ";
        break;
    case AS:
        retString = "<Assignment> -> ";
        break;
    case EX:
        retString = "<Expression> -> ";
        break;
    case EX_PRIME:
        retString = "<Expression Prime> -> ";
        break;
    case TE:
        retString = "<Term> -> ";
        break;
    case TE_PRIME:
        retString = "<Term Prime> -> ";
        break;
    case FA:
        retString = "<Factor> -> ";
        break;
    default:
        retString = "";
        break;
    }
    return retString;
}

//=======================================================================//
//    Description: This function will convert the predefined enum to a 
//                 string.
//
//    Input: symbol - predefined enum value 
//    Output: Returns a string with the right side of a production rule
//            , else empty string if the symbol is not a derivation
//======================================================================//
string Syntax::derivation_print_str(int symbol)
{
    string retString = "";
    switch (symbol)
    {
    case _AS:
        retString = "<Assignment>\n";
        break;
    case _ID_EQU_EX:
        retString = "<Identifier> = <Expression>\n";
        break;
    case _TE_EX_PRIME:
        retString = "<Term> <Expression Prime>\n";
        break;
    case _PLUS_TE_EX_PRIME:
        retString = "+ <Term> <Expression Prime>\n";
        break;
    case MINUS_TE_EX_PRIME:
        retString = "- <Term> <Expression Prime>\n";
        break;
    case _EPSILON:
        retString = "epsilon\n";
        break;
    case _FA_TE_PRIME:
        retString = "<Factor> <Term Prime>\n";
        break;
    case _MUL_FA_TE_PRIME:
        retString = "* <Factor> <Term Prime>\n";
        break;
    case _DIV_FA_TE_PRIME:
        retString = "/ <Factor> <Term Prime>\n";
        break;
    case _ID:
        retString = "<Identifier> \n";
        break;
    case _NUM:
        retString = "<Number>\n";
        break;
    case _LP_EX_RP:
        retString = "(<Expression>)\n";
        break;
    default:
        break;
    }
    return retString;
}

//=======================================================================//
//    Description: This function will convert the predefined enum to a 
//                 terminal string.
//
//    Input: symbol - predefined enum value 
//    Output: Returns a terminal string value, else empty string if the
//            symbol is not a terminal
//======================================================================//
string Syntax::term_to_string(int symbol)
{
    string retString = "";
    switch (symbol)
    {
    case ID:
        retString = "Identifier";
        break;
    case NUM:
        retString = "Valid Integer";
        break;
    case PLUS:
        retString = "+";
        break;
    case MINUS:
        retString = "-";
        break;
    case MUL:
        retString = "*";
        break;
    case DIV:
        retString = "/";
        break;
    case LP:
        retString = "(";
        break;
    case RP:
        retString = ")";
        break;
    case EQU:
        retString = "=";
        break;
    case SC:
        retString = ";";
        break;
    case DS:
        retString = "$";
        break;
    default:
        break;
    }
    return retString;
}

//=======================================================================//
//    Description: This function will convert a token/lexem to the proper
//                 column for the grammer table
//
//    Input: token - current token being inspected
//           lexem - current lexem being inspected
//    Output: Returns a column value in the grammar table, else returns -1
//======================================================================//
int Syntax::token_to_column(string token, string lexem)
{
    int column = -1;
    if(token == "Identifier")
    {
        column = 0;
    }
    else if(token == "Valid Integer" || token == "Valid Real")
    {
        column = 1;
    }
    else if(token == "Operators")
    {
        if(lexem[0] == '+')
        {
            column = 2;
        }
        else if(lexem[0] == '-')
        {
            column = 3;
        }
        else if(lexem[0] == '*')
        {
            column = 4;
        }
        else if(lexem[0] == '/')
        {
            column = 5;
        }
        else if(lexem[0] == '=')
        {
            column = 8;
        }
    }
    else if(token == "Seperators")
    {
        if(lexem[0] == '(')
        {
            column = 6;
        }
        else if(lexem[0] == ')')
        {
            column = 7;
        }
        else if(lexem[0] == ';')
        {
            column = 9;
        }
    }
    else
    {
        column = -1;
    }
    return column;
 }

//=======================================================================//
//    Description: This function compares the value on top of the stack 
//                 to the current token/lexem and return a boolean.
//
//    Input: enumVal - predefined value on top of the stack
//           token - current token being inspected
//           lexem - current lexem being inspected
//    Output: Returns true if the value on top of the stack is equal to 
//            the current token/lexem, else false
//======================================================================//
bool Syntax::comp_token_to_stack(int enumVal, string token, string lexem)
{
    bool is_equal = false;

    switch (enumVal)
    {
    case ID:
        is_equal = (token == "Identifier");
        break;

    case NUM:
        is_equal = (token == "Valid Integer" || token == "Valid Real");
        break;

    case PLUS:
        is_equal = (token == "Operators" && lexem[0] == '+');
        break;

    case MINUS:
        is_equal = (token == "Operators" && lexem[0] == '-');
        break;

    case MUL:
        is_equal = (token == "Operators" && lexem[0] == '*');
        break;
    case DIV:
        is_equal = (token == "Operators" && lexem[0] == '/');
        break;
    case EQU:
        is_equal = (token == "Operators" && lexem[0] == '=');
        break;
    case LP:
        is_equal = (token == "Seperators" && lexem[0] == '(');
        break;
    case RP:
        is_equal = (token == "Seperators" && lexem[0] == ')');
        break;
    case SC:
        is_equal = (token == "Seperators" && lexem[0] == ';');
        break;     
    default:
        is_equal = false;
        break;
    }
    return is_equal;
}
//=======================================================================//
//    Description: This function will convert a token/lexem to the proper
//                 column for the grammer table
//
//    Input: token - current token being inspected
//           lexem - current lexem being inspected
//    Output: Returns a column value in the grammar table, else returns -1
//======================================================================//
 void Syntax::der_reverse_push(int der)
{
    switch(der)
    {
        case _AS:
        currStack_.push(AS);
        break;

        case _ID_EQU_EX:
        currStack_.push(SC);
        currStack_.push(EX);
        currStack_.push(EQU);
        currStack_.push(ID);
        break;

        case _TE_EX_PRIME:
        currStack_.push(EX_PRIME);
        currStack_.push(TE);
        break;

        case _PLUS_TE_EX_PRIME:
        currStack_.push(EX_PRIME);
        currStack_.push(TE);
        currStack_.push(PLUS);
        break;

        case MINUS_TE_EX_PRIME:
        currStack_.push(EX_PRIME);
        currStack_.push(TE);
        currStack_.push(MINUS);
        break;

        case _FA_TE_PRIME:
        currStack_.push(TE_PRIME);
        currStack_.push(FA);
        break;

        case _MUL_FA_TE_PRIME:
        currStack_.push(TE_PRIME);
        currStack_.push(FA);
        currStack_.push(MUL);
        break;

        case _DIV_FA_TE_PRIME:
        currStack_.push(TE_PRIME);
        currStack_.push(FA);
        currStack_.push(DIV);
        break;

        case _ID:
        currStack_.push(ID);
        break;

        case _NUM:
        currStack_.push(NUM);
        break;

        case _LP_EX_RP:
        currStack_.push(RP);
        currStack_.push(EX);
        currStack_.push(LP);

        break;
    }
}

//=======================================================================//
//    Description: This function will check a predefined enum value and 
//                 determine if it is a terminal
//
//    Input: enumVal - a predfined enum value
//    Output: Returns true if the value is a terminal, else false
//======================================================================//
bool Syntax::is_token_terminal(int enumVal)
{
    bool is_terminal = false; 
    switch (enumVal)
    {
    case ID:
        is_terminal = true;
        break;
    case NUM:
        is_terminal = true;
        break;
    case PLUS:
        is_terminal = true;
        break;
    case MINUS:
        is_terminal = true;
        break;
    case MUL:
        is_terminal = true;
        break;
    case DIV:
        is_terminal = true;
        break;
    case LP:
        is_terminal = true;
        break;
    case RP:
        is_terminal = true;
        break;
    case EQU:
        is_terminal = true;
        break;
    case SC:
        is_terminal = true;
        break;
    case DS:
        is_terminal = true;
        break;
    default:
    is_terminal = false;
        break;
    }
    return is_terminal;
}

//=======================================================================//
//    Description: This function will check a predefined enum value and 
//                 determine if it is a nonterminal
//
//    Input: enumVal - a predfined enum value
//    Output: Returns true if the value is a nonterminal, else false
//======================================================================//
bool Syntax::is_token_non_terminal(int enumVal)
{
    bool is_non_terminal = false; 
    switch (enumVal)
    {
    case ST:
        is_non_terminal = true;
        break;
    case AS:
        is_non_terminal = true;
        break;
    case EX:
        is_non_terminal = true;
        break;
    case EX_PRIME:
        is_non_terminal = true;
        break;
    case TE:
        is_non_terminal = true;
        break;
    case TE_PRIME:
        is_non_terminal = true;
        break;
    case FA:
        is_non_terminal = true;
        break;
    default:
    is_non_terminal = false;
        break;
    }
    return is_non_terminal;
}
