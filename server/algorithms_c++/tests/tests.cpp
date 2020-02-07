#include <bits/stdc++.h>
#include "../grammars/Grammar.h"
#include "../relations/PrecedentialRelation.h"
#include "../syntax_analyzer/SyntaxAnalyzer.h"

using namespace std;

int main()
{
    Grammar *grammar = new Grammar("A,B,C|a,b,c,d|A|A,aB,A,Cd,B,c,B,ca,C,Cb,C,ba");
    PrecedentialRelation *precedentialRelation = new PrecedentialRelation(grammar);
    SyntaxAnalyzer *analyzer = new SyntaxAnalyzer(grammar);

    cout << precedentialRelation->atMostOneRelation() << endl;

    precedentialRelation->print();

    vector<string> commands = analyzer->shiftAndReduce("babbd");
    cout << commands.size() << endl;
    for (int i = 0; i < commands.size(); i++)
        cout << commands[i] << endl;

    delete grammar;
    delete precedentialRelation;
    delete analyzer;

    return 0;
}