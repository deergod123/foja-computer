#ifndef __PRECEDENTIALRELATION_H_
#define __PRECEDENTIALRELATION_H_
#include <bits/stdc++.h>

#include "../grammars/Grammar.h"

using namespace std;

class PrecedentialRelation
{
protected:
  set<pair<char, char>> lessRelation;
  set<pair<char, char>> equalsRelation;
  set<pair<char, char>> moreRelation;
  set<pair<char, char>> composition(set<pair<char, char>> relation1, set<pair<char, char>> relation2) const;
  set<pair<char, char>> transitiveClosure(set<pair<char, char>> relation, bool forFIRST, Grammar *grammar) const;
  set<pair<char, char>> inverseRelation(set<pair<char, char>> relation) const;

public:
  PrecedentialRelation(Grammar *grammar);
  bool isInLessRelation(pair<char, char> element) const;
  bool isInEqualsRelation(pair<char, char> element) const;
  bool isInMoreRelation(pair<char, char> element) const;
  bool atMostOneRelation() const;
  string getPrecedentialRelationString() const;
  void print() const;
};
#endif