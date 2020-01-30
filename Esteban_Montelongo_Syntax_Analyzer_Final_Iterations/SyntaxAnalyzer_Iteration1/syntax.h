//==================================== Table Driven Pridictive Parser Class ====================================//
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <stack>

using namespace std;

//================ Defining symbols that will be used for the parser ===============//
enum symbols {ST, AS, EX, EX_PRIME, TE, TE_PRIME, FA, // Nonterminals
ID, NUM, PLUS, MINUS, MUL, DIV, LP, RP, EQU, SC, DS, // Terminals
_AS, _ID_EQU_EX, _TE_EX_PRIME, _PLUS_TE_EX_PRIME, MINUS_TE_EX_PRIME,               // Derivations
_EPSILON , _FA_TE_PRIME, _MUL_FA_TE_PRIME, _DIV_FA_TE_PRIME, _ID, _NUM, _LP_EX_RP}; // Derivations

/*===================== Assignment Grammar Production List =====================//

ST -> AS
AS -> ID EQU EX SC
EX -> TE EX_PRIME
EX_PRIME -> PLUS TE EX_PRIME | MINUS TE EX_PRIME |_EPSILON
TE -> FA TE_PRIME
TE_PRIME -> MUL FA TE_PRIME | DIV FA TE_PRIME | _EPSILON
FA -> LP EX RP | ID | NUM

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
    bool is_token_terminal(int enumVal);
    bool is_token_non_terminal(int enumVal);
    void parser(const vector<string> &tokens,const vector<string> &lexems);
    void der_reverse_push(int);


    private:

    stack<int> currStack_;
    size_t currIndex_;
                                            // Predictive Parser Table
    vector<vector<int>> LL_Table_   /*S*/ {  {_AS, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                    /*AS*/    {_ID_EQU_EX, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                    /*EX*/    {_TE_EX_PRIME, _TE_EX_PRIME, -1, -1, -1, -1, _TE_EX_PRIME, -1, -1, -1, -1},
                                    /*EX_P*/  {-1, -1, _PLUS_TE_EX_PRIME, MINUS_TE_EX_PRIME, -1, -1, -1, _EPSILON, -1, _EPSILON, -1},
                                    /*TE*/    {_FA_TE_PRIME, _FA_TE_PRIME, -1, -1, -1, -1, _FA_TE_PRIME, -1, -1, -1, -1},
                                    /*TE_P*/  {-1, -1, _EPSILON, _EPSILON, _MUL_FA_TE_PRIME, _DIV_FA_TE_PRIME, -1, _EPSILON, -1, _EPSILON, -1},
                                    /*FA*/    {_ID, _NUM, -1, -1, -1, -1, _LP_EX_RP, -1, -1, -1, -1}  };

};


