#ifndef __FCWORD_H_
#define __FCWORD_H_
#include <bits/stdc++.h>

using namespace std;
#define NOSYMBOL 0
#define EPSSTR "<EPS>"
#define EPSSUB "*"

/*
	when using a set of words, a custom comparator is needed.
	using just set<Word*> does not work
	use wordset_t instead
*/
#define wordset_t set<Word*, wordcomp>

/*
	default value for insert/replace target checking
	set this to true for debug
*/
#define FCW_AUTOCHECKCONTAINS false

struct Symbol
{
	int id;   //symbol id, <0 for N, >0 for T, NOSYMBOL for no symbol
	Symbol* prev; //previous symbol, NULL if none
	Symbol* next; //next symbol, NULL if none
};


class Word
{
private:
	Symbol* start; //start symbol
	Symbol* end;  //end symbol
public:
	Word();  //empty word
	Word(Symbol* s);  //word from existing Symbol structure
	Word(int id);  //word containing a single symbol with given id
	Word(string repr);  //word from repr string, use only for grammar initialization
	void conc(Word* suf, bool cloned=true);  //concatenate words, modifies this, corrupts suf if not cloned
	Word* split(Symbol* where);  //split after where, modifies this, returns suffix
	void insert(Symbol* where, Word* what, bool cloned=true, bool check_where=FCW_AUTOCHECKCONTAINS); //insert what after where, where=NULL for start, modifies this, corrupts what if not cloned
	void insert(Symbol* where, Symbol* start, Symbol* end, bool cloned=true, bool check_where=FCW_AUTOCHECKCONTAINS); //insert symbol sequence after where, where=NULL for start, modifies this, corrupts start-end if not cloned
	void replace(Symbol* which, Word* what, bool cloned=true, bool check_what=FCW_AUTOCHECKCONTAINS); //replace which with what (apply rule), modifies this, corrupts what if not cloned
	void reverse();  //reverses the word
	bool equal(Word* w2); //compare word content
	bool contains(Symbol* s);  //returns true if s is a part of this' symbol structure
	bool less(Word* w2);  //comparator for wordcomp based on content id, can be used for ordering
	//Word* clone(Symbol* s=NULL, Symbol* e=NULL); //no longer supported
	Word* clone();  //returns a clone of this
	int length();  //returns word length
	Symbol* getStart();  //returns start symbol or NULL if empty
	Symbol* getEnd();  //returns end symbol or NULL if empty
	bool isEmpty();  //returns true if word is empty
	string toString();  //returns repr string, use only as representation/final output

};

class wordcomp  //comparator for wordset_t
{
public:
	wordcomp();
	bool operator() (Word* w1, Word* w2) const;
};



#endif
