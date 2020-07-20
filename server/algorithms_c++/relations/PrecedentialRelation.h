#ifndef __PRECEDENTIALRELATION_H_
#define __PRECEDENTIALRELATION_H_
#include <bits/stdc++.h>

#include "../grammars/Grammar.h"
#include "../words/Word.h"
using namespace std;

class PrecedentialRelation
{
protected:
  set<pair<int, int>> lessRelation;
  set<pair<int, int>> equalsRelation;
  set<pair<int, int>> moreRelation;
  set<pair<int, int>> composition(set<pair<int, int>> relation1, set<pair<int, int>> relation2) const;
  set<pair<int, int>> transitiveClosure(set<pair<int, int>> relation, bool forFIRST, Grammar *grammar) const;
  set<pair<int, int>> inverseRelation(set<pair<int, int>> relation) const;

public:
  PrecedentialRelation(Grammar *grammar);
  bool isInLessRelation(pair<int, int> element) const;
  bool isInEqualsRelation(pair<int, int> element) const;
  bool isInMoreRelation(pair<int, int> element) const;
  bool atMostOneRelation() const;
  string getPrecedentialRelationString() const;
  void print() const;
};
#endif
