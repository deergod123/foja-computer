#ifndef __FCWORD_H_
#define __FCWORD_H_
#include <bits/stdc++.h>

using namespace std;
#define NOSYMBOL 0
#define EPSSTR "<EPS>"
#define EPSSUB "*"
#define wordset_t set<Word*, wordcomp>


struct Symbol
{
	int id;
	Symbol* prev;
	Symbol* next;
};


class Word
{
private:
	Symbol* start;
	Symbol* end;
public:
	Word();
	Word(Symbol* s);
	Word(int id);
	Word(string repr);
	void conc(Word* suf, bool cloned=true);
	Word* split(Symbol* where);
	void insert(Symbol* where, Word* what, bool cloned=true);
	void insert(Symbol* where, Symbol* start, Symbol* end, bool cloned=true);
	void replace(Symbol* which, Word* what, bool cloned=true); //<-
	void reverse();
	bool equal(Word* w2);
	bool contains(Symbol* s);
	bool less(Word* w2);
	//Word* clone(Symbol* s=NULL, Symbol* e=NULL);
	Word* clone();
	int length();
	Symbol* getStart();
	Symbol* getEnd();
	bool isEmpty();
	string toString();

};

class wordcomp
{
public:
	wordcomp();
	bool operator() (Word* w1, Word* w2) const;
};


/*
auto wordcomp = [](Word* w1, Word* w2)
{
	return w1->equal(w2);
};
*/


#endif
