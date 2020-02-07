#include "../words/Word.h"

int main()
{
	////////////////
	//CONSTRUCTORS//
	////////////////
		cout << "constructors"<<endl;
		//epsilon
		Word* testeps=new Word();
		//single terminal "f"
		Word* teststf=new Word(6);
		//single nonterminal "D"
		Word* testsnd=new Word(-4);
		//single terminal 150
		Word* testt150=new Word(150);
		//single nonterminal -333
		Word* testn333=new Word(-333);
		//from symbol "abC(T150)"
		Symbol* s1=new Symbol();
		Symbol* s=s1;
		Symbol* s2=new Symbol();
		s1->prev=NULL;
		s1->next=s2;
		s1->id=1;
		s2->id=2;
		s2->prev=s1;
		s1=new Symbol();
		s1->prev=s2;
		s2->next=s1;
		s1->id=-3;
		s2=new Symbol();
		s2->prev=s1;
		s1->next=s2;
		s2->next=NULL;
		s2->id=150;
		Word* testlong=new Word(s);
		//print
		cout<<"<eps> "<<testeps ->toString()<<endl;
		cout<<"f "<<teststf ->toString()<<endl;
		cout<<"D "<<testsnd ->toString()<<endl;
		cout<<"(T150) "<<testt150 ->toString()<<endl;
		cout<<"(N333) "<<testn333 ->toString()<<endl;
		cout<<"abC(T150) "<<testlong ->toString()<<endl;
	///////////
	//cloning//
	///////////
		cout<<"cloning"<<endl;
		Word* testlongclone=testlong ->clone();
		cout << testlong ->toString() << " at "<<testlong<<" , "<<testlongclone ->toString()<<" at "<<testlongclone;
		string cmp= (testlong==testlongclone)?"eq":"diff";
		cout<<" "<<cmp<<endl;
		Word* testepsclone=testeps->clone();
		cout<<testeps ->toString()<<" "<<testepsclone ->toString()<<endl;
	////////
	//conc//
	////////
		cout<<"conc"<<endl;
		testlong->conc(testn333,true);
		testlong->conc(testlongclone,false);
		cout << "abC(T150)(N333)abC(T150) "<<testlong ->toString()<<endl;
	/////////
	//split//
	/////////
		cout<<"split"<<endl;
		Symbol* ts=testlong->getStart();
		for(int i=0;i<5;i++)
		{
			ts=ts->next;
		}
		Word* testsplit=testlong->split(ts);
		Word* printw=new Word(ts->id);
		cout<<"splitting on "<< printw ->toString()<<endl;
		cout<<"pref "<<testlong ->toString()<<endl;
		cout<<"suf "<<testsplit ->toString()<<endl;
		testlong->conc(testsplit,false);
		cout<<"conc "<<testlong ->toString()<<endl;
	//////////
	//insert//
	//////////
		cout<<"insert"<<endl;
		Word* testins=new Word(-2);
		ts=testlong->getStart();
		for(int i=0;i<3;i++)
		{
			ts=ts->next;
		}
		testlong->insert(ts,testins);
		printw=new Word(ts->id);
		cout<<"inserting "<<testins ->toString()<<" at "<<printw ->toString()<<endl;
		cout<<"abC(T150)B(N333)abC(T150) "<<testlong ->toString()<<endl;
	///////////
	//replace//
	///////////
		cout<<"replace"<<endl;
		ts=ts->next;
		Word* testrepl=new Word(2);
		testrepl->conc(new Word(3),false);
		cout<<"replacing B with "<<testrepl ->toString()<<endl;
		testlong->replace(ts,testrepl);
		cout<<"abC(T150)bc(N333)abC(T150) "<<testlong ->toString()<<endl;
	//////////
	//length//
	//////////
		cout<<"length"<<endl;
		cout<<"eps "<<testeps ->length()<<endl;
		cout<<"f "<<teststf ->length()<<endl;
		cout<<"abC(T150)bc(N333)abC(T150) [11] "<<testlong ->length()<<endl;
	cout<<"Test complete."<<endl;
	return 0;
}
