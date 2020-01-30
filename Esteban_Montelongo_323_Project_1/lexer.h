#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>

using namespace std;


class Lexer
{

    public:
    Lexer();
    bool readFile(string fileName);
    void print();
    

    private:
    size_t currSize_;
    size_t currIndex_;
    int currState_;
    bool isComment_;
    string sourceCode_;
    vector<string> token_;
    vector<string> lexems_;
                                      // L  D  $  .  Se  Op  !
    vector<vector<int>> stateTable_ = { {1,  2, 10, 8, 8, 9, 4},
                                        {1,  1, 1, 10, 5, 10, 10},
                                        {10, 2, 10, 3, 6, 10, 10},
                                        {10, 3, 10, 10, 7, 10, 10},
                                        {4,  4, 4, 4, 4, 4, 4, 11}};
    void parser();
    bool isOperator(char c);
    bool isSeperator(char c);
    bool isKeyWord(string test);
    int checkInputs(char c);

};


