#ifndef __GRAMMAR_H_
#define __GRAMMAR_H_
#include <bits/stdc++.h>
#include "../words/Word.h"

using namespace std;


class PrecedentialRelation;

class Grammar
{
private:
  void printr(pair<int,Word*> r); //test only!
protected:
  set<int> nonterminals;
  set<int> terminals;
  int start;
  set<pair<int, Word*>> rules;
  //int minimum(queue<pair<Word*, int>> &queue);
  //int minimum(set<Word*> words);
  //int countOfTerminals(Word* w);

public:
  set<int> getNonterminals() const;
  set<int> getTerminals() const;
  int getStart() const;
  set<pair<int, Word*>> getRules() const;
  Grammar(set<int> n,set<int> t,set<pair<int,Word*>> p, int s);
  //Grammar(string stringGrammar);
  string hello() const;
  set<pair<int,Word*>> getRulesFromNonterminal(int nt);
  //void removeLeftRecursion();
  void toChomskyNormalForm();
  //void toReducedNormalForm();
  //void toEpsilonFreeForm();
  void toGreibachNormalForm();
  //string isEquivalent(Grammar* grammar);
  //string grammarToString();
  //bool isPrecedential();
  //string getYourPrecedentialRelationString();
  string testprint();
};
#endif
