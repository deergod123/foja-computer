#ifndef __SYNTAXANALYZER_H_
#define __SYNTAXANALYZER_H_
#include <bits/stdc++.h>

#include "../grammars/Grammar.h"
#include "../words/Word.h"
#include "../relations/PrecedentialRelation.h"

//#define END_OF_WORD '#' //unused
#define BOTTOM_OF_STACK NOSYMBOL
#define CMD_ACCEPT 1
#define CMD_REJECT 0
#define CMD_SHIFT 2
#define CMD_REDUCE 3


using namespace std;


struct Command
{
	int value;
	Word* currentWord;
	rule_t* reduceRule;
};

class SyntaxAnalyzer
{
protected:
    Grammar *grammar;
    Word* getCurrentWord(stack<int> stk, Word* w, Symbol* currentSymbol) const;
    void shift(stack<int> &stk, Word* &w, Symbol** currentSymbol, vector<Command*> &commands);
    bool reduce(stack<int> &stk, PrecedentialRelation* &precedentialRelation, Word* &w, Symbol* &currentSymbol, vector<Command*> &commands);
    bool shiftAndReduceAnalyzed(stack<int> &stk, Symbol* &s);
    void printCommands(vector<Command*> commands);
public:
    SyntaxAnalyzer(Grammar *grammar);
    vector<Command*> shiftAndReduce(Word* w);
    string getShiftAndReduceString(vector<Command*> commands, string separator="|");
};
#endif
