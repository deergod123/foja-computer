#ifndef __GRAMMAR_H_
#define __GRAMMAR_H_
#include <bits/stdc++.h>
#include "../words/Word.h"

using namespace std;


class PrecedentialRelation;

class Grammar
{
private:
  void printr(pair<int,Word*> r); //print single rule
protected:
  set<int> nonterminals;
  set<int> terminals;
  int start;
  set<pair<int, Word*>> rules;
  int minimum(queue<pair<Word*, int>> &queue);
  int minimum(WordSet* words);
  int countOfTerminals(Word* w);

public:
  set<int> getNonterminals() const;
  set<int> getTerminals() const;
  int getStart() const;
  set<pair<int, Word*>> getRules() const;
  Grammar(set<int> n,set<int> t,set<pair<int,Word*>> p, int s); //construct from N,T,P,S
  Grammar(string stringGrammar); //construct from string
  string hello() const;
  set<pair<int,Word*>> getRulesFromNonterminal(int nt); //returns set of rules from given N
  pair<bool,bool> patchNonterminals(); //add used and missing N, remove present and unused N, returns pair<added,removed>
  bool removeDupedRules(); //remove duplicate rules
  //void removeLeftRecursion();
  void toChomskyNormalForm();
  void toReducedNormalForm();
  void toEpsilonFreeForm();
  void toGreibachNormalForm(); //TODO
  string isEquivalent(Grammar* grammar);
  string grammarToString(); //returns stringGrammar
  bool isPrecedential();
  string getYourPrecedentialRelationString();
  string testprint(); //prints out grammar N,T,P,S
};
#endif

