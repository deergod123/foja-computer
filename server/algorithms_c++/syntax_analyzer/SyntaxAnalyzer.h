#ifndef __SYNTAXANALYZER_H_
#define __SYNTAXANALYZER_H_
#include <bits/stdc++.h>

#include "../grammars/Grammar.h"
#include "../relations/PrecedentialRelation.h"

#define END_OF_WORD '#'
#define BOTTOM_OF_STACK '#'

using namespace std;

class SyntaxAnalyzer
{
protected:
    Grammar *grammar;
    string getCurrentWord(stack<char> stackOfChars, const string word, int pointerOnCurrentChar) const;
    void shift(stack<char> &stackOfChars, const string &word, int &pointerOnCurrentChar, vector<string> &commands);
    bool reduce(stack<char> &stackOfChars, PrecedentialRelation *precedentialRelation, const string &word, const int &pointerOnCurrentChar, vector<string> &commands);
    bool shiftAndReduceAnalyzed(stack<char> &stackOfChars, const string &word, int &pointerOnCurrentChar);
    void printCommands(vector<string> commands) const;

public:
    SyntaxAnalyzer(Grammar *grammar);
    vector<string> shiftAndReduce(string word);
    string getShiftAndReduceString(vector<string> commands);
};
#endif