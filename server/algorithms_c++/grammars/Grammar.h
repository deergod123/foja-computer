#ifndef __GRAMMAR_H_
#define __GRAMMAR_H_
#include <bits/stdc++.h>
#include "../words/Word.h"

using namespace std;
#define rule_t pair<int, Word*>
#define ruleset_t set<rule_t, rulecomp >

class PrecedentialRelation;

class rulecomp
{
public:
	rulecomp();
	bool operator() (rule_t r1, rule_t r2) const;
};


class Grammar
{
private:
  void printr(rule_t r); //print single rule
protected:
  set<int> nonterminals;
  set<int> terminals;
  int start;
  ruleset_t rules;
  int minimum(queue<pair<Word*, int>> &queue);
  int minimum(wordset_t words);
  int countOfTerminals(Word* w);

public:
  set<int> getNonterminals() const;
  set<int> getTerminals() const;
  int getStart() const;
  ruleset_t getRules() const;
  Grammar(set<int> n,set<int> t,ruleset_t p, int s); //construct from N,T,P,S
  Grammar(string stringGrammar); //construct from string
  string hello() const;
  ruleset_t getRulesFromNonterminal(int nt); //returns set of rules from given N
  pair<bool,bool> patchNonterminals(); //add used and missing N, remove present and unused N, returns pair<added,removed>
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

