#include "Word.h"
using namespace std;

/*
comparator for wordset_t (set of words)
*/
wordcomp::wordcomp()
{

}

bool wordcomp::operator() (Word* w1, Word* w2) const
{
	if(w1==w2)
	{
		return true;
	}
	if(w1==NULL)
	{
		return false;
	}
	return w1->less(w2);
}

/*
class for words
basically a two-way linked list of ints
Symbol -> id for symbol value, negative values represent nonterminals,
positive (nonzero) values represent terminals, 0 represents no symbol
*/

Word::Word() //creates an empty word
{
	this->start=NULL;
	this->end=NULL;
}

Word::Word(Symbol* s) //creates a word from an existing symbol structure
{
	this->start=s;
	if(s==NULL) //check for eps
	{
		this->end=NULL;
	}
	else //find end
	{
		for(Symbol* i=s;;i=i->next)
		{
			if(i->next==NULL)
			{
				this->end=i;
				break;
			}
		}
		s->prev=NULL;

	}

}

Word::Word(int id) //creates a word containing a single character with given id
{
	Symbol* s=new Symbol();
	s->prev=NULL;
	s->next=NULL;
	s->id=id;
	this->start=s;
	this->end=s;
}

Word::Word(string repr) //creates a word from repr string, see Word::toString() for details
{
	cout<<"Constructing word from "<<repr<<endl;
	if(repr==EPSSTR || repr==EPSSUB) //check for empty word
	{
		this->start=NULL;
		this->end=NULL;
		return;
	}
	bool high_read=false; //reading a high symbol
	bool high_terminate=false; //can terminate high read
	bool high_terminal=false; //reading high terminal/nonterminal
	stringstream high_stream; //stream for symbol id
	high_stream.clear();
	Symbol* outw=NULL;
	Symbol* prev=NULL;
	for(int i=0;i<repr.length();i++)
	{
		if(high_read) //currently reading a high symbol
		{
			if(high_terminate) //can terminate
			{
				if(repr[i]==')') //terminated
				{
					string high_string=high_stream.str(); //used only for checking
					if(!high_string.length())
					{
						cerr<<"invalid high symbol format, empty id"<<endl;
						exit(1);
					}
					int id;
					high_stream >> id;
					id*=(high_terminal ? 1 : -1); //switch to negative if nonterminal
					Symbol* s=new Symbol(); //create and link symbol
					s->id=id;
					s->prev=prev;
					s->next=NULL;
					if(prev!=NULL)
					{
						prev->next=s;
					}
					if(outw==NULL)
					{
						outw=s;
					}
					prev=s;
					high_read=false; //high read has ended
					high_terminate=false; //high read can no longer terminate
					high_stream.clear(); //clear for further use
					continue;
				}
				if(repr[i]>='0' && repr[i]<='9') //did not terminate, check for number
				{
					high_stream << repr[i]; //add number to stream
					continue;
				}
				cerr<< "invalid high symbol format, not a number id"<<endl;
				exit(1);
			}
			//cannot terminate, detect N/T
			if(repr[i]=='N')
			{
				high_terminal=false;
				high_terminate=true;
				continue;
			}
			if(repr[i]=='T')
			{
				high_terminal=true;
				high_terminate=true;
				continue;
			}
			cerr<< "invalid high symbol format, missing T/N"<<endl;
			exit(1);
		}
		//not in high read
		if(repr[i]=='(') //check for high read
		{
			high_read=true;
				continue;
		}
		if(repr[i]>='A' && repr[i]<='Z') //check for low nonterminal
		{
			int id = (1+(int)repr[i]-(int)'A')*-1; //convert to int
			Symbol* s=new Symbol(); //create and link symbol
			s->id=id;
			s->prev=prev;
			s->next=NULL;
			if(prev!=NULL)
			{
				prev->next=s;
			}
			if(outw==NULL)
			{
				outw=s;
			}
			prev=s;
			continue;
		}
		if(repr[i]>='a' && repr[i]<='z') //check for low terminal
		{
			int id = (1+(int)repr[i]-(int)'a'); //convert to int
			Symbol* s=new Symbol(); //create and link symbol
			s->id=id;
			s->prev=prev;
			s->next=NULL;
			if(prev!=NULL)
			{
				prev->next=s;
			}
			if(outw==NULL)
			{
				outw=s;
			}
			prev=s;
			continue;
		}
		cerr<<"invalid word format, unknown symbol '"<<repr[i]<<"'"<<endl;
		exit(1);
	}
	if(high_read) //finished reading input, but a high read is open (missing ")")
	{
		cerr<<"unfinished high read"<<endl;
		exit(1);
	}
	if(outw==NULL) //used "" for epsilon
	{
		cerr<<"invalid epsilon representation, use "<<EPSSTR<<" or "<<EPSSUB<<"."<<endl;
		exit(1);
	}
	//finalize
	this->start=outw;
	this->end=prev;

}
///////////

void Word::conc(Word* suf, bool cloned) //concatenate words
{
	if(suf==NULL)
	{
		cerr<<"conc with NULL suffix"<<endl;
		exit(1);
	}
	Word* w=suf;
	if(cloned)
	{
		w=suf->clone();
	}

	if(this->isEmpty()) //conc. empty word with w is w
	{
		this->start=w->getStart();
		this->end=w->getEnd();
		return;
	}
	if(suf->isEmpty()) //conc. with w yields no changes
	{
		return;
	}
	//link end to start
	this->end->next=w->getStart();
	w->getStart()->prev=this->end;
	this->end=w->getEnd(); //update end
}

Word* Word::split(Symbol* where) //split after this symbol, create new word from suffix, where=NULL defaults to start
{
	if(where==NULL) //default to start
	{
		if(this->isEmpty())
		{
			return new Word();
		}

		Word* w=new Word(this->start); //create a copy (not a clone) of this
		this->start=NULL; //set this to epsilon
		this->end=NULL;
		return w;
	}
	Word* w=new Word(where->next); //create new word from where->next. Note that this delinks where->next->prev, but NOT where->next
	this->end=where; //update end
	where->next=NULL; //manually delink
	return w;
}

void Word::insert(Symbol* where, Word* what,bool cloned, bool check_where) //insert word after this symbol, null defaults to start
//used word is no longer usable if not cloned
//if check_where is true, check whether where is a part of this word
//	use only for testing, requires an extra iteration
{
	if(what==NULL)
	{
		cerr << "inserting null" <<endl;
		exit(1);
	}
	if(what==this && !cloned) //inserting this w/o cloning would create a cyclic link
	{
		cerr<<"inserting this without cloning"<<endl;
		exit(1);
	}
	if(what->isEmpty()) //inserting an empty word is trivial
	{
		return;
	}
	Word* w=what;
	if(cloned) //check for clone
	{
		w=what->clone();
	}
	if(this->isEmpty()) //inserting into an empty word is just copying (not cloning) w
	{
		if(where!=NULL)
		{
			cerr<<"inserting at an actual symbol into an empty word"<<endl;
			exit(1);
		}
		this->start=w->getStart();
		this->end=w->getEnd();
		return;
	}
	if(where==NULL) //default to start
	{
		//link what->end to this->start
		Symbol* ss=this->start;
		this->start=what->getStart(); //update
		what->getEnd()->next=ss;
		ss->prev=what->getEnd();
		return;
	}
	if(where==this->end) //insert at end
	{
		//link this->end to what->start
		where->next=w->getStart();
		w->getStart()->prev=where;
		this->end=w->getEnd(); //update
		return;
	}
	if(check_where && !this->contains(where)) //check for invasive insertion
	{
		cerr<<"inserting at a symbol not in this word"<<endl;
		exit(1);
	}

	//link where to what->start and what->end to where->next
	w->getEnd()->next=where->next;
	where->next->prev=w->getEnd();
	where->next=w->getStart();
	where->next->prev=where;
}


void Word::insert(Symbol* where, Symbol* start, Symbol* end, bool cloned, bool check_where) //insert symbol sequence after where, where=NULL defaults to start
//symb. seq. gets corrupted if not cloned
{
	cout<<"t: "<<this->toString()<<endl;
	if(start==NULL || end==NULL) //empty sequence
	{
		return;
	}
	//unlink sequence, create backups
	Symbol* backupstart=start->prev;
	Symbol* backupend=end->next;
	start->prev=NULL;
	end->next=NULL;
	Word* insword=new Word(start); //wrap unlinked sequence as a new word
	if(cloned) //check for clone
	{

		insword=insword->clone();
		//relink to restore sequence
		start->prev=backupstart;
		end->next=backupend;

	}
	this->insert(where,insword,false,check_where); //insert wrapped sequence as a word
}


void Word::replace(Symbol* which, Word* what, bool cloned, bool check_which) //replace a symbol with what (apply grammar rule)
//what is corrupted if not cloned
//if check_which is true, checks whether which is an actual symbol in this word
//	use only for testing, requires an extra iteration
{
	if(what==NULL)
	{
		cerr << "replacing with null" <<endl;
		exit(1);
	}

	if(what==this && !cloned) //would create a cyclic link
	{
		cerr<<"replacing with this without cloning"<<endl;
		exit(1);
	}

	if(which==NULL)
	{
		cerr << "replacing null" <<endl;
		exit(1);
	}

	if(check_which && !this->contains(which)) //check for invasive insertion
	{
		cerr<<"replacing a symbol not in this word"<<endl;
		exit(1);
	}


	Word* w=what;
	if(cloned) //check for clone
	{
		w=what->clone();
	}
	if(what->isEmpty()) //replacing with epsilon, delete which
	{
		if(which->prev!=NULL) //which!=start
		{
			which->prev->next=which->next;
		}
		else
		{
			this->start=which->next;
		}
		if(which->next==NULL) //which=end
		{
			this->end=which->prev;
		}
		else
		{
			which->next->prev=which->prev;
		}
		return;
	}
	if(which->prev==NULL) //which=start
	{
		this->start=w->getStart();
	}
	else
	{
		which->prev->next=w->getStart();
		w->getStart()->prev=which->prev;
	}
	if(which->next==NULL) //which=end
	{
		this->end=w->getEnd();
	}
	else
	{
		which->next->prev=w->getEnd();
		w->getEnd()->next=which->next;
	}
}

bool Word::equal(Word* w2) //compares word content
{
	if(w2==NULL)
		return false;
	if(w2==this)
	{
		return true;
	}
	Symbol* s2=w2->getStart();
	if(s2==this->start) //equal start symbol=equal content
	{
		return true;
	}
	for(Symbol* s=this->start;;s=s->next) //iterate over both words
	{
		if(s==NULL) //reached this->end
		{
			return (s2==NULL); //true if other has ended too
		}
		if(s2==NULL) //other ended first
		{
			return false;
		}
		if(s->id!=s2->id) //no end, compare id
		{
			return false;
		}
		s2=s2->next; //iter. w2
	}
}

bool Word::less(Word* w2) //compare words in id order, used by wordcomp
//ordering rules:
//1) N<T
//2) (Sx)<(Sx+1) for S in (N,T)
//3) shorter<longer
{
	if(w2==NULL)
		return true;
	if(w2==this) //this!<this
	{
		return false;
	}
	Symbol* s2=w2->getStart();
	if(s2==this->start) //same start, same content, not less
	{
		return false;
	}
	for(Symbol* s=this->start;;s=s->next) //iter over both words
	{
		if(s==NULL) //reached this->end
		{
			return !(s2==NULL); //true if w2 is longer
		}
		if(s2==NULL)
		{
			return false; //w2 is shorter, therefore w2<this
		}
		if(s->id!=s2->id) //different symbol
		{
			if(s->id<0 && s2->id>0)
			{
				return true; //N<T
			}
			if(s->id>0 && s2->id<0)
			{
				return false; //T>N
			}
			return abs(s->id)< abs(s2->id); //compare id
		}
		s2=s2->next;
	}
}

Word* Word::clone() //create a "clone" of this word
//returns a new word with the same content but located elsewhere in memory
{
	if(this->isEmpty())
	{
		return new Word();
	}

	Symbol* p=NULL;
	Symbol* st=NULL;
	for(Symbol* i=this->start;;i=i->next) //iterate over this
	{
		if(i==NULL)
		{
			break;
		}
		Symbol* s=new Symbol(); //for copying
		if(st==NULL)
		{
			st=s;
		}
		s->prev=p; //link to last copied
		s->next=NULL; //unlink next in case this is the last symbol
		s->id=i->id; //copy id
		if(p!=NULL) //not the first copied symbol
		{
			p->next=s; //link back
		}
		p=s; //iter. copying
	}
	return new Word(st); //finalize

}

Symbol* Word::getStart()
{
	return this->start;
}
Symbol* Word::getEnd()
{
	return this->end;
}
bool Word::isEmpty() //true if this=epsilon
{
	return(this->start==NULL || this->end==NULL);
}
int Word::length() //returns word length.
//!DO NOT USE FOR ITERATING OVER THIS WORD!
//as this function iterates over this anyway
//iterate as a linked list instead
{
	int o=0;
	for(Symbol* s=this->start;s!=NULL;s=s->next) //iterate over this, count symbols
	{
		o++;
	}
	return o;
}
string Word::toString() //convert to string, use only for representation/final output
{
	if(this->isEmpty())
	{
		return EPSSTR;
	}
	stringstream ss;
	for(Symbol* i=this->start;i!=NULL;i=i->next) //iter over this
	{
		if(i->id>0) //terminal
		{
			if(i->id>26) //high terminal
			{
				ss << "(T" << i->id << ")";
				continue;
			}
			//low terminal
			char c=static_cast<char>(96+i->id);
			ss << c;
			continue;
		}
		if(i->id<0) //nonterminal
		{
			if(i->id<-26) //high nonterminal
			{
				ss << "(N" << (i->id)*-1 <<")";
				continue;
			}
			//low nonterminal
			char c=static_cast<char>(64-i->id);
			ss << c;
		}
		//NOSYMBOL is ignored
	}
	return ss.str();
}

void Word::reverse() //reverse this word, e.g. abc -> cba
{
	Symbol* ss=NULL;
	for(Symbol* s=this->start;s!=NULL;s=s->prev) //iter
	{
		//swap links
		ss=s->next;
		s->next=s->prev;
		s->prev=ss;
	}
	//swap start, end
	ss=this->start;
	this->start=this->end;
	this->end=ss;
}


bool Word::contains(Symbol* s) //returns true if s is a symbol in this word
//checks for actual pointers, not id
{
	if(this->isEmpty() && s==NULL) //NULL is always in an empty word, but never in a non-empty one
	{
		return true;
	}
	for(Symbol* ss=this->start;ss!=NULL;ss=ss->next) //look for s
	{
		if(ss==s) //found
		{
			return true;
		}
	}
	//not found
	return false;
}
