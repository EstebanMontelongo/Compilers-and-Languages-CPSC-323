//==================================== Table Driven Pridictive Parser Class ====================================//
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <stack>

using namespace std;

//================ Defining symbols that will be used for the parser ===============//
enum symbols {ST, DE, TY, MID, // Nonterminals
SC, INT, FLOAT, BOOL, COMMA, ID, DS, // Terminals
_DE, _EPSILON, _INT, _FLOAT, _BOOL, _TY_ID_MID_SC, _COMMA_ID_MID}; // Derivations

/*===================== Declaration Grammar Production List =====================//

ST -> <DE>
DE -> <TY> ID <MID> SC | _EPSILON
TY -> INT | FLOAT | BOOL
MID -> COMMA ID <MID> | _EPSILON

*/

class Syntax
{

    public:
    Syntax(); 
    string non_term_print_str(int symbol);
    string derivation_print_str(int symbol);
    string term_to_string(int symbol);
    int token_to_column(string token, string lexem);
    bool comp_token_to_stack(int enumVal, string token, string lexem);
    bool is_terminal(int enumVal);
    bool is_non_terminal(int enumVal);
    void parser(const vector<string> &tokens,const vector<string> &lexems);
    void der_reverse_push(int);


    private:

    stack<int> currStack_;
    size_t currIndex_;
                                            // Predictive Parser Table
    vector<vector<int>> LL_Table_   /*ST*/ {  {-1, _DE, _DE, _DE, -1, -1, _EPSILON},
                                    /*DE*/    {_EPSILON, _TY_ID_MID_SC, _TY_ID_MID_SC, _TY_ID_MID_SC, -1, -1, _EPSILON},
                                    /*TY*/    {-1, _INT, _FLOAT, _BOOL, -1, -1, -1},
                                    /*MID*/   {-1, -1, -1, -1, _EPSILON, _COMMA_ID_MID, -1} };

};


