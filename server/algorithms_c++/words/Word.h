#ifndef __FCWORD_H_
#define __FCWORD_H_
#include <bits/stdc++.h>

using namespace std;
#define NOSYMBOL 0
#define EPSSTR "<EPS>"
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
	void conc(Word* suf, bool cloned=true);
	Word* split(Symbol* where);
	void insert(Symbol* where, Word* what, bool cloned=true);
	void replace(Symbol* which, Word* what, bool cloned=true); //<-
	bool equal(Word* w2);
	//Word* clone(Symbol* s=NULL, Symbol* e=NULL);
	Word* clone();
	int length();
	Symbol* getStart();
	Symbol* getEnd();
	bool isEmpty();
	string toString();

};
#endif
