#include "Word.h"
using namespace std;

Word::Word() //eps
{
	this->start=NULL;
	this->end=NULL;
}

Word::Word(Symbol* s) //non-eps
{
	this->start=s;
	s->prev=NULL;
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
	if(repr==EPSSTR)
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
						cout<<"invalid high symbol format, empty id"<<endl;
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
				cout<< "invalid high symbol format, not a number id"<<endl;
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
			cout<< "invalid high symbol format, missing T/N"<<endl;
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
		cout<<"invalid word format, unknown symbol '"<<repr[i]<<"'"<<endl;
		exit(1);
	}
	if(high_read)
	{
		cout<<"unfinished high read"<<endl;
		exit(1);
	}
	if(outw==NULL)
	{
		cout<<"invalid epsilon representation, use "<<EPSSTR<<"."<<endl;
		exit(1);
	}
	this->start=outw;
	this->end=prev;

}
///////////

void Word::conc(Word* suf, bool cloned) //add suffix to word
{
	Word* w=suf;
	if(cloned)
	{
		w=suf->clone();
	}
	//cout<<"cloned"<<endl;
	if(suf==NULL)
	{
		//cout<<"conc with suffix NULL"<<endl;
		exit(1);
	}
	if(this->end==NULL) //eps
	{
		//cout<<"i am epsilon"<<endl;
		this->start=w->getStart();
		this->end=w->getEnd();
		//cout<<"finished"<<endl;
		return;
	}
	//cout<<"i am not epsilon!!"<<endl;
	if(suf->isEmpty())
	{
		//cout<<"other is epsilon"<<endl<<"finished"<<endl;
		return;
	}
	//cout<<"other is not epsilon"<<endl;
	//cout<<(this ->end == NULL)<<endl;
	this->end->next=w->getStart();
	//cout<<"1"<<endl;
	w->getStart()->prev=this->end;
	this->end=w->getEnd();
	//cout<<"finished"<<endl;
}

Word* Word::split(Symbol* where) //split after this symbol, create new word from suffix
{
	if(where==NULL)
	{
		cout << "splitting on null or splitting epsilon" <<endl;
		exit(1);
	}
	Word* w=new Word(where->next);
	this->end=where;
	where->next->prev=NULL;
	where->next=NULL;
	return w;
}

void Word::insert(Symbol* where, Word* what,bool cloned) //insert word after this symbol, if null and eps defaults to start
//used word is no longer usable if not cloned
{
	if(what==NULL)
	{
		cout << "inserting null" <<endl;
		exit(1);
	}
	if(what==this)
	{
		cout<<"inserting this"<<endl;
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
		cout << "inserting on null into a non-empty word"<<endl;
		exit(1);
	}
	if(where==this->start)
	{
		this->start->prev=w->getEnd();
		this->start=w->getStart();
		w->getEnd()->next=this->start;
		return;
	}
	if(where==this->end)
	{
		this->end->next=w->getStart();
		w->getStart()->prev=this->end;
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
		cout << "inserting null" <<endl;
		exit(1);
	}
	if(what==this)
	{
		cout<<"inserting this"<<endl;
		exit(1);
	}
	Word* w=what;
	if(cloned)
	{
		w=what->clone();
	}
	if(which==NULL)
	{
		cout << "inserting at null" <<endl;
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
			this->end=NULL;
		}
		if(which->next!=NULL)
		{
			which->next->prev=which->prev;
		}
		else
		{
			this->end=which->prev;
		}
		if(which->prev==NULL)
		{
			this->start=NULL;
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
	Symbol* s2=w2->getStart();
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
			char c {96+i->id};
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
			char c {64-i->id};
			ss << c;
			
		}
	}
	return ss.str();
}







WordSet::WordSet()
{

}

WordSet::WordSet(set<Word*> words, bool wrap)
{
	if(wrap)
	{
		this->words=words;
	}
	else
	{
		this->words.insert(words.begin(),words.end());
	}
}

set<Word*> WordSet::getSet()
{
	return this->words;
}

bool WordSet::contains(Word* w)
{
	for(auto ww:this->words)
	{
		if(ww->equal(w))
		{
			return true;
		}
	}
	return false;
}

void WordSet::insert(Word* w)
{
	if(this->contains(w))
	{
		return;
	}
	this->words.insert(w);
}
