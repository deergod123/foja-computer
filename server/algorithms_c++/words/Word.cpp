#include "Word.h"
using namespace std;


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

Word::Word() //eps
{
	this->start=NULL;
	this->end=NULL;
}

Word::Word(Symbol* s) //non-eps
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

Word::Word(int id) //single character from id
{
	Symbol* s=new Symbol();
	s->prev=NULL;
	s->next=NULL;
	s->id=id;
	this->start=s;
	this->end=s;
}

Word::Word(string repr) //from string
{
	cout<<"Constructing word from "<<repr<<endl;
	if(repr==EPSSTR || repr==EPSSUB)
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
		if(high_read)
		{
			if(high_terminate)
			{
				if(repr[i]==')')
				{
					string high_string=high_stream.str();
					if(!high_string.length())
					{
						cerr<<"invalid high symbol format, empty id"<<endl;
						exit(1);
					}
					int id;
					high_stream >> id;
					id*=(high_terminal ? 1 : -1);
					Symbol* s=new Symbol();
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
					high_read=false;
					high_terminate=false;
					high_stream.clear();
					continue;
				}
				if(repr[i]>='0' && repr[i]<='9')
				{
					high_stream << repr[i];
					continue;
				}
				cerr<< "invalid high symbol format, not a number id"<<endl;
				exit(1);
			}
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
		if(repr[i]=='(')
		{
			high_read=true;
				continue;
		}
		if(repr[i]>='A' && repr[i]<='Z')
		{
			int id = (1+(int)repr[i]-(int)'A')*-1;
			Symbol* s=new Symbol();
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
		if(repr[i]>='a' && repr[i]<='z')
		{
			int id = (1+(int)repr[i]-(int)'a');
			Symbol* s=new Symbol();
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
	if(high_read)
	{
		cerr<<"unfinished high read"<<endl;
		exit(1);
	}
	if(outw==NULL)
	{
		cerr<<"invalid epsilon representation, use "<<EPSSTR<<" or "<<EPSSUB<<"."<<endl;
		exit(1);
	}
	this->start=outw;
	this->end=prev;

}
///////////

void Word::conc(Word* suf, bool cloned) //add suffix to word
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
	if(this->isEmpty()) //eps
	{
		this->start=w->getStart();
		this->end=w->getEnd();
		return;
	}
	if(suf->isEmpty())
	{
		return;
	}
	this->end->next=w->getStart();
	w->getStart()->prev=this->end;
	this->end=w->getEnd();
}

Word* Word::split(Symbol* where) //split after this symbol, create new word from suffix
{
	if(where==NULL)
	{
		if(this->isEmpty())
		{
			return new Word();
		}
		Word* w=new Word(this->start);
		this->start=NULL;
		this->end=NULL;
		return w;
	}
	Word* w=new Word(where->next);
	this->end=where;
	where->next=NULL;
	return w;
}

void Word::insert(Symbol* where, Word* what,bool cloned) //insert word after this symbol, null defaults to start
//used word is no longer usable if not cloned
{
	if(what==NULL)
	{
		cerr << "inserting null" <<endl;
		exit(1);
	}
	if(what==this && !cloned)
	{
		cerr<<"inserting this without cloning"<<endl;
		exit(1);
	}
	if(what->isEmpty())
	{
		return;
	}
	Word* w=what;
	if(cloned)
	{
		w=what->clone();
	}
	if(this->start==NULL)
	{
		this->start=w->getStart();
		this->end=w->getEnd();
		return;
	}
	if(where==NULL)
	{
		Symbol* ss=this->start;
		this->start=what->getStart();
		what->getEnd()->next=ss;
		ss->prev=what->getEnd();
		return;
	}
	if(where==this->end)
	{
		where->next=w->getStart();
		w->getStart()->prev=where;
		this->end=w->getEnd();
		return;
	}
	w->getEnd()->next=where->next;
	where->next->prev=w->getEnd();
	where->next=w->getStart();
	where->next->prev=where;
}

void Word::replace(Symbol* which, Word* what, bool cloned)
{
	if(what==NULL)
	{
		cerr << "replacing with null" <<endl;
		exit(1);
	}
	if(what==this && !cloned)
	{
		cerr<<"replacing with this without cloning"<<endl;
		exit(1);
	}
	Word* w=what;
	if(cloned)
	{
		w=what->clone();
	}
	if(which==NULL)
	{
		cerr << "replacing null" <<endl;
		exit(1);
	}
	if(what->isEmpty())
	{
		if(which->prev!=NULL)
		{
			which->prev->next=which->next;
		}
		else
		{
			this->start=which->next;
		}
		if(which->next==NULL)
		{
			this->end=which->prev;
		}
		else
		{
			which->next->prev=which->prev;
		}
		return;
	}
	if(which->prev==NULL)
	{
		this->start=w->getStart();
	}
	else
	{
		which->prev->next=w->getStart();
		w->getStart()->prev=which->prev;
	}
	if(which->next==NULL)
	{
		this->end=w->getEnd();
	}
	else
	{
		which->next->prev=w->getEnd();
		w->getEnd()->next=which->next;
	}
}

bool Word::equal(Word* w2)
{
	if(w2==NULL)
		return false;
	if(w2==this)
	{
		return true;
	}
	Symbol* s2=w2->getStart();
	if(s2==this->start)
	{
		return true;
	}
	for(Symbol* s=this->start;;s=s->next)
	{
		if(s==NULL)
		{
			return (s2==NULL);
		}
		if(s2==NULL)
		{
			return false;
		}
		if(s->id!=s2->id)
		{
			return false;
		}
		s2=s2->next;
	}
}

bool Word::less(Word* w2)
{
	if(w2==NULL)
		return true;
	if(w2==this)
	{
		return false;
	}
	Symbol* s2=w2->getStart();
	if(s2==this->start)
	{
		return false;
	}
	for(Symbol* s=this->start;;s=s->next)
	{
		if(s==NULL)
		{
			return !(s2==NULL); //w2 is longer
		}
		if(s2==NULL)
		{
			return false; //w2 is shorter
		}
		if(s->id!=s2->id) //different symbol
		{
			if(s->id<0 && s2->id>0)
			{
				return true; //N<T
			}
			if(s->id>0 && s2->id<0)
			{
				return false;
			}
			return abs(s->id)< abs(s2->id);
		}
		s2=s2->next;
	}
}

Word* Word::clone()
{
	if(this->isEmpty())
	{
		return new Word();
	}
	Symbol* p=NULL;
	Symbol* st=NULL;
	for(Symbol* i=this->start;;i=i->next)
	{
		if(i==NULL)
		{
			break;
		}
		Symbol* s=new Symbol();
		if(st==NULL)
		{
			st=s;
		}
		s->prev=p;
		s->next=NULL;
		s->id=i->id;
		if(p!=NULL)
		{
			p->next=s;
		}
		p=s;
	}
	return new Word(st);
	

}

Symbol* Word::getStart()
{
	return this->start;
}
Symbol* Word::getEnd()
{
	return this->end;
}
bool Word::isEmpty()
{
	return(this->start==NULL || this->end==NULL);
}
int Word::length()
{
	int o=0;
	for(Symbol* s=this->start;s!=NULL;s=s->next)
	{
		o++;
	}
	return o;
}
string Word::toString()
{
	if(this->isEmpty())
	{
		return EPSSTR;
	}
	stringstream ss;
	for(Symbol* i=this->start;i!=NULL;i=i->next)
	{
		if(i->id>0)
		{
			if(i->id>26)
			{
				ss << "(T" << i->id << ")";
				continue;
			}
			char c=static_cast<char>(96+i->id);
			ss << c;
			continue;
		}
		if(i->id<0)
		{
			if(i->id<-26)
			{
				ss << "(N" << (i->id)*-1 <<")";
				continue;
			}
			char c=static_cast<char>(64-i->id);
			ss << c;
		}
	}
	return ss.str();
}

void Word::reverse()
{
	Symbol* ss=NULL;
	for(Symbol* s=this->start;s!=NULL;s=s->prev)
	{
		ss=s->next;
		s->next=s->prev;
		s->prev=ss;
	}
	ss=this->start;
	this->start=this->end;
	this->end=ss;
}

void Word::insert(Symbol* where, Symbol* start, Symbol* end, bool cloned)
{
	cout<<"t: "<<this->toString()<<endl;
	if(start==NULL || end==NULL)
	{
		return;
	}
	Symbol* backupstart=start->prev;
	Symbol* backupend=end->next;
	start->prev=NULL;
	end->next=NULL;
	Word* insword=new Word(start);
	if(cloned)
	{

		insword=insword->clone();
		start->prev=backupstart;
		end->next=backupend;

	}
	this->insert(where,insword);
}

bool Word::contains(Symbol* s)
{
	if(this->isEmpty() && s==NULL)
	{
		return true;
	}
	for(Symbol* ss=this->start;ss!=NULL;ss=ss->next)
	{
		if(ss==s)
		{
			return true;
		}
	}
	return false;
}
