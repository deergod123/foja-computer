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
	int id;   //symbol id, <0 for N, >0 for T, 0 for no symbol
	Symbol* prev;
	Symbol* next;
};


class Word
{
private:
	Symbol* start;
	Symbol* end;
public:
	Word();  //empty word
	Word(Symbol* s);  //word from existing Symbol structure
	Word(int id);  //word containing a single symbol with given id
	Word(string repr);  //word from repr string, use only for init
	void conc(Word* suf, bool cloned=true);  //concatenate words
	Word* split(Symbol* where);  //split after where, returns suffix
	void insert(Symbol* where, Word* what, bool cloned=true); //insert what after where, where=NULL for start
	void insert(Symbol* where, Symbol* start, Symbol* end, bool cloned=true); //insert symbol sequence after where, where=NULL for start
	void replace(Symbol* which, Word* what, bool cloned=true); //replace which with what (apply rule)
	void reverse();
	bool equal(Word* w2); //compare word content
	bool contains(Symbol* s);  //returns true if s is a part of this'symbol structure
	bool less(Word* w2);  //comparator for wordcomp
	//Word* clone(Symbol* s=NULL, Symbol* e=NULL);
	Word* clone();  //returns a clone of this
	int length();  //returns word length
	Symbol* getStart();
	Symbol* getEnd();
	bool isEmpty();
	string toString();  //returns repr string, ose only as representation/final output

};

class wordcomp  //comparator for wordset_t
{
public:
	wordcomp();
	bool operator() (Word* w1, Word* w2) const;
};



#endif
