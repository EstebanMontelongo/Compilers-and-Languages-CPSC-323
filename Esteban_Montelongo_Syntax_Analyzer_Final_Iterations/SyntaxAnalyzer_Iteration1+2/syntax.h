//==================================== Table Driven Pridictive Parser Class ====================================//
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <stack>

using namespace std;


//===================== Defining Symbols Assign =====================//
enum symbols {ST, AS, EX, EX_PRIME, TE, TE_PRIME, FA, // Nonterminals
ID_ASS, NUM, PLUS, MINUS, MUL, DIV, LP, RP, EQU, SC_ASS, DS, // Terminals
_AS, _ID_EQU_EX, _TE_EX_PRIME, _PLUS_TE_EX_PRIME, MINUS_TE_EX_PRIME, // Derivations
_EPSILON_ASS , _FA_TE_PRIME, _MUL_FA_TE_PRIME, _DIV_FA_TE_PRIME, _ID, _NUM, _LP_EX_RP, // Derivations
ST_DEC, DE, TY, MID, // Nonterminals
SC_DEC, INT, FLOAT, BOOL, COMMA, ID, // Terminals
_DE, _EPSILON_DEC, _INT, _FLOAT, _BOOL, _TY_ID_MID_SC, _COMMA_ID_MID};// Derivations


/*===================== Declaration Grammar Production List =====================//

ST -> <DE>
DE -> <TY> ID <MID> SC | _EPSILON
TY -> INT | FLOAT | BOOL
MID -> COMMA ID <MID> | _EPSILON

*/

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
    int token_to_column_dec(string token, string lexem);
    int token_to_column_ass(string token, string lexem);
    bool comp_token_to_stack(int enumVal, string token, string lexem);
    bool is_terminal(int enumVal);
    bool is_non_terminal(int enumVal);
    void parser(const vector<string> &tokens,const vector<string> &lexems);
    void der_reverse_push(int);


    private:

    stack<int> currStack_;
    size_t currIndex_;
                                            // Predictive Parser Table
    vector<vector<int>> Dec_Table_   /*ST*/ {  {-1, _DE, _DE, _DE, -1, -1, _EPSILON_ASS},
                                    /*DE*/    {_EPSILON_ASS, _TY_ID_MID_SC, _TY_ID_MID_SC, _TY_ID_MID_SC, -1, -1, _EPSILON_ASS},
                                    /*TY*/    {-1, _INT, _FLOAT, _BOOL, -1, -1, -1},
                                    /*MID*/   {-1, -1, -1, -1, _EPSILON_ASS, _COMMA_ID_MID, -1} };
    
                                            // Predictive Parser Table
    vector<vector<int>> Ass_Table_   /*ST*/ {  {_AS, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                    /*AS*/    {_ID_EQU_EX, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                    /*EX*/    {_TE_EX_PRIME, _TE_EX_PRIME, -1, -1, -1, -1, _TE_EX_PRIME, -1, -1, -1, -1},
                                    /*EX_P*/  {-1, -1, _PLUS_TE_EX_PRIME, MINUS_TE_EX_PRIME, -1, -1, -1, _EPSILON_DEC, -1, _EPSILON_DEC, -1},
                                    /*TE*/    {_FA_TE_PRIME, _FA_TE_PRIME, -1, -1, -1, -1, _FA_TE_PRIME, -1, -1, -1, -1},
                                    /*TE_P*/  {-1, -1, _EPSILON_DEC, _EPSILON_DEC, _MUL_FA_TE_PRIME, _DIV_FA_TE_PRIME, -1, _EPSILON_DEC, -1, _EPSILON_DEC, -1},
                                    /*FA*/    {_ID, _NUM, -1, -1, -1, -1, _LP_EX_RP, -1, -1, -1, -1}  };


};


