#include "../words/Word.h"
#include "../grammars/Grammar.h"
using namespace std;

int main()
{
	cout<<"CNF test"<<endl;

	set<int> nts;
	nts.insert(-1);
	nts.insert(-2);
	nts.insert(-19);


	set<int> ts;
	ts.insert(1);
	ts.insert(2);


	set<pair<int,Word*>> rs;
	pair<int,Word*> rp;


	Word* wrd=new Word(1);
	wrd->conc(new Word(-1),false);
	wrd->conc(new Word(2),false);
	wrd->conc(new Word(-2),false);
	rp.first=-19;
	rp.second=wrd;
	rs.insert(rp);


	wrd=new Word(-2);
	wrd->conc(new Word(-2),false);
	rp.first=-19;
	rp.second=wrd;
	rs.insert(rp);


	wrd=new Word(-1);
	rp.first=-19;
	rp.second=wrd;
	rs.insert(rp);


	wrd=new Word(-1);
	wrd->conc(new Word(2),false);
	wrd->conc(new Word(-1),false);
	rp.first=-1;
	rp.second=wrd;
	rs.insert(rp);


	wrd=new Word(1);
	wrd->conc(new Word(1),false);
	rp.first=-1;
	rp.second=wrd;
	rs.insert(rp);


	wrd=new Word(2);
	wrd->conc(new Word(-2),false);
	wrd->conc(new Word(-2),false);
	rp.first=-2;
	rp.second=wrd;
	rs.insert(rp);


	wrd=new Word();
	rp.first=-2;
	rp.second=wrd;
	rs.insert(rp);

/*	for(auto rule:rs)
	{
		cout<<rule.first<<" -> "<<rule.second ->toString()<<endl;
	}
*/

	Grammar* g=new Grammar(nts,ts,rs,-19);


	cout<< g->testprint()<<endl;
	cout<<"CNF"<<endl;
	g->toChomskyNormalForm();
	cout<< g->testprint()<<endl;

	cout<<"Test complete."<<endl;
	return 0;
}
