#include "Grammar.h"
//#include "../relations/PrecedentialRelation.h"
#include "../words/Word.h"


/////////////////////////////////////////////////////////////////
set<int> Grammar::getNonterminals() const
{
    return this->nonterminals;
}

set<int> Grammar::getTerminals() const
{
    return this->terminals;
}

int Grammar::getStart() const
{
    return this->start;
}

set<pair<int, Word*>> Grammar::getRules() const
{
    return this->rules;
}
///////////////////////////
///////////////////////////

Grammar::Grammar(set<int> n,set<int> t,set<pair<int,Word*>> p, int s)
{
	/*
	this->nonterminals=n;
	this->terminals=t;
	this->rules=p;
	this->start=s;
	*/
	this->start=s;
	//cout<<"start set"<<endl;
	for(auto nt:n)
	{
		//cout<<nt<<" ";
		this->nonterminals.insert(nt);
	}
	//cout<<endl<<"nonterminals set"<<endl;
	for(auto ter:t)
	{
		//cout<<ter<<" ";
		this->terminals.insert(ter);
	}
	//cout<<endl<<"terminals set"<<endl;
	for(auto rule:p)
	{
		pair<int,Word*> rp;
		//cout<<rule.first<<" -> ";
		rp.first=rule.first;
		//cout<<rule.second->toString()<<endl;
		rp.second=rule.second->clone();
		this->rules.insert(rp);
	}
	//cout<<"complete"<<endl;

}
//

/*
Grammar::Grammar(string stringGrammar)
{
    stringstream stringGrammarStream(stringGrammar);
    string segment;
    vector<string> grammar;

    while (getline(stringGrammarStream, segment, '|'))
    {
        grammar.push_back(segment);
    }

    stringstream nonterminalsStream(grammar[0]);
    while (getline(nonterminalsStream, segment, ','))
    {
        this->nonterminals.insert(segment[0]);
    }

    stringstream terminalsStream(grammar[1]);
    while (getline(terminalsStream, segment, ','))
    {
        this->terminals.insert(segment[0]);
    }

    this->start = grammar[2][0];

    stringstream rulesStream(grammar[3]);
    pair<string, string> rule;
    rule.first = epsilon;
    rule.second = epsilon;
    while (getline(rulesStream, segment, ','))
    {
        if (rule.first == epsilon)
        {
            rule.first = segment;
        }
        else
        {
            rule.second = segment;
            if (rule.second == substitutedEpsilon)
                rule.second = epsilon;
            this->rules.insert(rule);
            rule.first = epsilon;
            rule.second = epsilon;
        }
    }
}

void Grammar::toReducedNormalForm()
{
    for (auto rule : this->rules)
        if ((rule.second!=NULL)&&(rule.second.getStart()!=NULL)&&(rule.first == rule.second.getStart()->id ) && (rule.second.getStart()->next == NULL))
            this->rules.erase(rule);

    set<int> terminatedNonterminals;

    while (true)
    {
        bool end = true;
        for (auto rule : this->rules)
        {
            bool terminatedRule = true;
            //
			for (unsigned int i = 0; i < rule.second.length(); i++)
            {
                if ((terminatedNonterminals.count(rule.second[i]) == 0) && (this->terminals.count(rule.second[i]) == 0))
                {
                    terminatedRule = false;
                    break;
                }
            }
			//
			for(Symbol* s=rule.second.getStart();s!=NULL;s=s->next)
			{
				if((terminatedNonterminals.count(s->id)==0) && (this->terminals.count(s->id)==0))
				{
					terminatedRule=false;
					break;
				}
			}
			//
            if ((terminatedRule) && (terminatedNonterminals.count(rule.first) == 0))
            {
                terminatedNonterminals.insert(rule.first);
                end = false;
            }
        }
        if (end)
            break;
    }
    for (auto rule : this->rules)
    {
        if (terminatedNonterminals.count(rule.first) == 0)
        {
            this->rules.erase(rule);
            continue;
        }
		/
        for (unsigned int i = 0; i < rule.second.length(); i++)
            if ((terminatedNonterminals.count(rule.second[i]) == 0) && (this->terminals.count(rule.second[i]) == 0))
            {
                this->rules.erase(rule);
                break;
            }
		/
		for(Symbol* s=rule.second.getStart();s!=NULL;s=s->next)
		{
			if((terminatedNonterminals.count(s->id)==0) && (this->terminals.count(s->id)==0))
			{
				this->rules.erase(rule);
				break;
			}
		}
    }
    terminatedNonterminals.clear();

    set<int> achievableNonterminals;
    achievableNonterminals.insert(this->start);
    queue<int> q;
    q.push(this->start);
    while (!q.empty())
    {
        int word = q.front();
        q.pop();
        for (auto rule : this->rules)
        {
            if (rule.first == word)
            {
				/
                for (unsigned int i = 0; i < rule.second.length(); i++)
                {
                    if ((achievableNonterminals.count(rule.second[i]) == 0) && (this->terminals.count(rule.second[i]) == 0))
                    {
                        achievableNonterminals.insert(rule.second[i]);
                        q.push(rule.second[i]);
                    }
                }
				/
				for(Symbol* s=rule.second.getStart();s!=NULL;s=s->next)
				{
					if((achievableNonterminals.count(s->id)==0) && (this->terminals.count(s->id)==0))
					{
						achievableNonterminals.insert(s->id);
						q.push(s->id);
					}
				}
            }
        }
    }

    for (auto rule : this->rules)
    {
        if (achievableNonterminals.count(rule.first) == 0)
        {
            this->rules.erase(rule);
            continue;
        }
		/
        for (unsigned int i = 0; i < rule.second.length(); i++)
            if ((achievableNonterminals.count(rule.second[i]) == 0) && (this->terminals.count(rule.second[i]) == 0))
            {
                this->rules.erase(rule);
                break;
            }
		/
		for(Symbol* s=rule.second.getStart();s!=NULL;s=s->next)
		{
			if((achievableNonterminals.count(s->id)==0)&&(this->terminals.count(s->id)==0))
			{
				this->rules.erase(rule):
				break;
			}
		}
    }

    achievableNonterminals.clear();
}




							/////////////////////////////////////
/////////////////////////////       							////////////////////////////////////////////////
							/////////////////////////////////////





void Grammar::toEpsilonFreeForm()
{
    set<int> removableNonterminals;
    for (auto rule : this->rules)
        if (rule.second.isEmpty())
            removableNonterminals.insert(rule.first->id);

    //finding out removable rules
    while (true)
    {
        bool end = true;
        for (auto rule : this->rules)
        {
            bool removableRule = true;
			/
            for (unsigned int i = 0; i < rule.second.length(); i++)
            {
                if (removableNonterminals.count(rule.second[i]) == 0)
                {
                    removableRule = false;
                    break;
                }
            }
			/
			for(Symbol* s=rule.second.getStart();s!=NULL;s=s->next)
			{
				if(removableNonterminals.count(s->id)==0)
				{
					removableRule=false;
					break;
				}
			}
            if ((removableRule) && (removableNonterminals.count(rule.first) == 0))
            {
                removableNonterminals.insert(rule.first);
                end = false;
            }
        }
        if (end)
            break;
    }

    //computing new rules
    set<pair<int, Word*>> newRules;

    for (auto rule : this->rules)
    {
        string combination = "";
		/
        for (unsigned int i = 0; i < rule.second.length(); i++)
            if (removableNonterminals.count(rule.second[i]) != 0)
                combination += "0";
		/
		for(Symbol* s=rule.second.getStart();s!=NULL;s=s->next)
		{
			if(removableNonterminals.count(s->id)!=0)
			{
				combination+=0;
			}
		}
        if (combination == "")
            continue;

        while (count(combination.begin(), combination.end(), '1') != combination.length())
        {
            Word* newRule = new Word();
            int removableNonterminal = 0;
			/
            for (unsigned int i = 0; i < rule.second.length(); i++)
            {
                if (removableNonterminals.count(rule.second[i]) != 0)
                {
                    if (combination[removableNonterminal] == '1')
                        newRule += rule.second[i];
                    removableNonterminal++;
                }
                else
                    newRule += rule.second[i];
            }
			/
			for(Symbol* s=rule.second.getStart();s!=NULL;s=s->next)
			{
				if(removableNonterminals.count(s->id)!=0)
				{
					if(combination[removableNonterminal]=='1')
					{
						Symbol* ss=new Symbol();
							ss->prev=NULL;
							ss->next=NULL;
							ss->id=s->id;
						newrule.conc(new Word(ss),false);
					}
					removableNonterminal++;
				}
				else
				{
					Symbol* ss=new Symbol();
						ss->prev=NULL;
						ss->next=NULL;
						ss->id=s->id;
					newrule.conc(new Word(ss),false);
				}
			}
            for (int i = combination.length() - 1; i >= 0; i--)
            {
                combination[i] = combination[i] == '0' ? '1' : '0';
                if (combination[i] == '1')
                    break;
            }

            pair<int, Word*> newRulePair;
            newRulePair.first = rule.first;
            newRulePair.second = newRule;
            newRules.insert(newRulePair);
        }
    }

    //inserting new rules
    for (auto newRule : newRules)
        this->rules.insert(newRule);

    //erasing epsilon rules
    for (auto rule : this->rules)
        if (rule.second.isEmpty())
            this->rules.erase(rule);
}




int Grammar::countOfTerminals(Word* w)
{
    int count = 0;
	/
	for (unsigned int i = 0; i < word.length(); i++)
        if ((((int)word[i]) >= 97) && (((int)word[i]) <= 122))
            count++;
    /
	for(Symbol* s=w.getStart();s!=NULL;s=s->next)
	{
		count+= (s->id>0 ? 1:0);
	}
	return count;
}

int Grammar::minimum(queue<pair<Word*, int>> &queue)
{
	//what?
    int minimum = 0;
    int size = queue.size();
    for (unsigned int i = 0; i < size; i++)
    {
        pair<Word*, int> rule = queue.front();
        queue.pop();
        if (i == 0)
            minimum = rule.second;
        else if (minimum > rule.second)
            minimum = rule.second;
        queue.push(rule);
    }
    return minimum;
}

int Grammar::minimum(set<Word*> words)
{
    int minimum = 0, i = 0;
    for (auto word : words)
    {
        if (i == 0)
        {
            minimum = word.length();
            i++;
        }
        else if (minimum > word.length())
            minimum = word.length();
    }
    return minimum;
}

string Grammar::isEquivalent(Grammar *grammar)
{
    int maxMemory = 0;
    int reachedSize = 0;
    queue<pair<Word*, int>> thisQueue, grammarQueue;
    set<Word*> thisWords, grammarWords;
    pair<Word*, int> start;

    start.first = new Word(this->start);
    start.second = 0;
    thisQueue.push(start);

    start.first = new Word(grammar->start);
    start.second = 0;
    grammarQueue.push(start);

    maxMemory += 2;

    this->toReducedNormalForm();
    grammar->toReducedNormalForm();

    for (unsigned int i = 0; i < 10000; i++)
    {
        if (i % 1000 == 0)
        {
            int thisMinimum = thisQueue.size() == 0 ? this->minimum(thisWords) : this->minimum(thisQueue);
            int grammarMinimum = grammarQueue.size() == 0 ? this->minimum(grammarWords) : this->minimum(grammarQueue);

            reachedSize = max(reachedSize, min(thisMinimum, grammarMinimum));
            for (auto word : thisWords)
            {
                if ((word.length() < min(thisMinimum, grammarMinimum)) && (grammarWords.count(word) == 0))
                    return word.isEmpty() ? "Epsilon|0" : word.toString() + "|0";
            }

            for (auto word : grammarWords)
            {
                if ((word.length() < min(thisMinimum, grammarMinimum)) && (thisWords.count(word) == 0))
                    return word.isEmpty() ? "Epsilon|0" : word.toString() + "|0";
            }
        }

        pair<Word*, int> thisWord;
        if (!thisQueue.empty())
        {
            thisWord = thisQueue.front();
            thisQueue.pop();
        }

        maxMemory -= thisWord.first.length();

        pair<Word*, int> grammarWord;
        if (!grammarQueue.empty())
        {
            grammarWord = grammarQueue.front();
            grammarQueue.pop();
        }

        maxMemory -= grammarWord.first.length();
		/
        for (unsigned int j = 0; j < thisWord.first.length(); j++)
        {
            if ((int)thisWord.first[j] > 90)
                continue;
            for (auto rule : this->rules)
            {
                if (rule.first == string(1, thisWord.first[j]))
                {
                    pair<string, int> newPair = thisWord;
                    newPair.first.erase(j, 1);
                    newPair.first.insert(j, rule.second);
                    newPair.second += this->countOfTerminals(rule.second);

                    if (newPair.second == newPair.first.length())
                        thisWords.insert(newPair.first);
                    else
                        thisQueue.push(newPair);
                    if (maxMemory + newPair.first.length() > 50000000)
                        return to_string(reachedSize) + "|1";
                    else
                        maxMemory += newPair.first.length();
                }
            }
        }
		TODO/
		
        for (unsigned int j = 0; j < grammarWord.first.length(); j++)
        {
            if ((int)grammarWord.first[j] > 90)
                continue;
            for (auto rule : grammar->rules)
            {
                if (rule.first == string(1, grammarWord.first[j]))
                {
                    pair<string, int> newPair = grammarWord;
                    newPair.first.erase(j, 1);
                    newPair.first.insert(j, rule.second);
                    newPair.second += grammar->countOfTerminals(rule.second);

                    if (newPair.second == newPair.first.length())
                        grammarWords.insert(newPair.first);
                    else
                        grammarQueue.push(newPair);
                    if (maxMemory + newPair.first.length() > 50000000)
                        return to_string(reachedSize) + "|1";
                    else
                        maxMemory += newPair.first.length();
                }
            }
        }
    }

    return to_string(reachedSize) + "|1";
}

string Grammar::grammarToString()
{
    string stringGrammar = epsilon;
    for (auto nonterminal : this->nonterminals)
    {
        stringGrammar += string(1, nonterminal) + ",";
    }
    stringGrammar.pop_back();
    stringGrammar += "|";
    for (auto terminal : this->terminals)
    {
        stringGrammar += string(1, terminal) + ",";
    }
    stringGrammar.pop_back();
    stringGrammar += "|" + string(1, this->start);
    stringGrammar += "|";
    for (auto rule : this->rules)
    {
        stringGrammar += rule.first + ",";
        if (rule.second == substitutedEpsilon)
            stringGrammar += ",";
        else
            stringGrammar += rule.second + ",";
    }
    stringGrammar.pop_back();
    return stringGrammar;
}

bool Grammar::isPrecedential()
{
    // there cant be the rule with epsilon
    for (auto rule : this->rules)
        if (rule.second.isEmpty())
            return false;

    // there cant be more than one rules with the same right side
    /
	map<string, int> amountOfRules;
    for (auto rule : this->rules)
    {
        amountOfRules[rule.second]++;
        if (amountOfRules[rule.second] > 1)
            return false;
    }
	/
	for(auto rule:this->rules)
	{
		for(auto rule2:this->rules)
		{
			if((rule!=rule2) && (rule.second.equal(rule2.second)))
			{
				return false;
			}
		}
	}
    // there can't be more than one relations betwen 2 symbols from N merged with P
    PrecedentialRelation *precedentialRelation = new PrecedentialRelation(this);
    bool result = precedentialRelation->atMostOneRelation();
    delete precedentialRelation;
    return result;
}

string Grammar::getYourPrecedentialRelationString()
{
    PrecedentialRelation *precedentialRelation = new PrecedentialRelation(this);
    string precedentialRelationString = precedentialRelation->getPrecedentialRelationString();
    delete precedentialRelation;
    return precedentialRelationString;
}
*/
string Grammar::hello() const
{
    return "hello world!";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////EDIT









set<pair<int,Word*>> Grammar::getRulesFromNonterminal(int nt)
{
	if(!(this->nonterminals.count(nt)))
	{
		cout << "Grammar does not contain nonterminal id " << nt <<endl;
	}
	set<pair<int,Word*>> o;
	for(auto rule:this->rules)
	{
		if(rule.first==nt)
		{
			pair<int,Word*> np;
			np.first=rule.first;
			np.second=rule.second;
			o.insert(np);
		}
	}
	return o;
}

/*
void Grammar::removeLeftRecursion()
{
	int nextnewnt=-1;
	set<int> addednt;
	for(int n:this->nonterminals)
	{
		if(n<=nextnewnt)
		{
			nextnewnt=n-1;
		}
	}
	for(int nt:this->nonterminals)
	{
		set<pair<int,Word*>> ntrules=this.getRulesFromNonterminal(nt);
		set<pair<int,Word*>> ntrulesok;
		for(auto rule:ntrules)
		{
			if(rule.second.isEmpty() || rule.second.getStart()->id!=rule.first)
			{
				ntrulesok.insert(rule);
				ntrules.erase(ntrules.find(rule));
			}
		}
		if(ntrules.size()==0) //all rules are non-leftrec
		{
			continue;
		}
		set<pair<int,Word*>> ntrulesnew;
		addednt.insert(nextnewnt);
		Symbol* newnt=new Symbol();
		newnt->prev=NULL;
		newnt->next=NULL;
		newnt->id=nextnewnt;
		Word* newntw=new Word(newnt);
		for(auto rule:ntrulesok)
		{
			this->rules.erase(this->rules.find(rule));
			pair<int,Word*> nr;
			nr.first=nt;
			nr.second=rule.second.clone().conc(newntw,true);
			ntrulesnew.insert(nr);
		}
		for(auto rule:ntrules)
		{
			this->rules.erase(this->rules.find(rule));
			pair<int,Word*> nr;
			nr.first=nextnewnt;
			nr.second=rule.second.split(rule.second.getStart()).conc(newntw,true);
			ntrulesnew.insert(nr);
		}
		pair<int,Word*> er;
		er.first=nextnewnt;
		er.second=new Word();
		ntrulesnew.insert(er);
		for(auto rule:ntrulesnew)
		{
			this->rules.insert(rule);
		}
		nextnewnt--;
	}
}



*/


void Grammar::toChomskyNormalForm()
{
	//
	set<pair<int,Word*>> xitr;
	int nextnewnt=-1;
	for(int n:this->nonterminals)
	{
		if(n<=nextnewnt)
		{
			nextnewnt=n-1;
		}
	}
//cout<<"xdrf"<<endl;
	for(int t:this->terminals)
	{
		pair<int,Word*> nxitrp;
		nxitrp.first=nextnewnt;
		nextnewnt--;
		this->nonterminals.insert(nxitrp.first);
		Symbol* xits=new Symbol();
		xits->id=t;
		xits->prev=NULL;
		xits->next=NULL;
		Word* xitw=new Word(xits);
		nxitrp.second=xitw;
		xitr.insert(nxitrp);
	}
//cout<<"xdrf"<<endl;
	
	for(auto rule:this->rules) //swap terminals w/ new nt
	{
		Symbol* s=rule.second->getStart();
		if(s==NULL) //N->eps
		{
			continue;
		}
		if((s->id>0) && (s->next==NULL)) //N->T
		{
			continue;
		}
		for(;s!=NULL;s=s->next)
		{
			if(s->id>0)
			{
				for(auto xirule:xitr)
				{
					if((xirule.second->getStart()->id)==s->id)
					{
						s->id=xirule.first;
						break;
					}
				}
			}
		}
	}
//cout<<"xdrf"<<endl;
	
	pair<int,Word*> xieps; //N->eps
	xieps.first=nextnewnt;
	nextnewnt--;
	this->nonterminals.insert(xieps.first);
	xieps.second=new Word();
	/*
	Symbol* es=new Symbol();
	es->prev=NULL;
	es->next=NULL;
	es->id=xieps.first;
	//updated for id-specific constructor
	*/
	Word* xiepsw=new Word(xieps.first); //word containing only xi_eps (for conc)
	set<pair<int,Word*>> seqr;
//cout<<"xdrf"<<endl;

	for(auto rule:this->rules)
	{
		//cout<<"rule ";
		//this->printr(rule);
		if(rule.second->isEmpty()) //N->eps
		{
			seqr.insert(rule);
			continue;
		}
		if((rule.second->getStart()->id)>0) //N->T
		{
			seqr.insert(rule);
			continue;
		}
		Symbol* s=rule.second->getStart();
		if(s->next==NULL) //chain rules
		{
			rule.second->conc(xiepsw,true);
			seqr.insert(rule);
			continue;
		}
		if(s->next->next==NULL) //N->NN
		{
			seqr.insert(rule);
			continue;
		}
		//N->NNN+
		int lastnt=rule.first;
		for(Word* ww=rule.second; ; )
		{
	//cout<<"split"<<endl;
			Word* suf=ww->split(ww->getStart());
			pair<int,Word*> nseqrp;
			nseqrp.first=lastnt;
			nseqrp.second=ww;
			/*
			Symbol* ss=new Symbol();
			ss->prev=NULL;
			ss->next=NULL;
			ss->id=nextnewnt;
			lastnt=nextnewnt;
			nextnewnt--;
			//updated
			*/
			this->nonterminals.insert(lastnt);
	//cout<<"conc"<<endl;
			ww->conc(new Word(nextnewnt),false);
			lastnt=nextnewnt;
			this->nonterminals.insert(lastnt);
			nextnewnt--;
			//nseqrp.second=ww;
			seqr.insert(nseqrp);
	//cout<<"suf "<<suf ->toString() <<endl;
			if(suf->length()==2)
			{
	//cout<<"break"<<endl;
				pair<int,Word* > eseqrp;
				eseqrp.first=lastnt;
				eseqrp.second=suf;
				//this->printr(eseqrp);
				seqr.insert(eseqrp);
				break;
			}
			if(suf->length()<2)
			{
				cout<<"something ain't right, suffix <2 characters "<<suf ->toString()<<endl;
				exit(1);
			}
	//cout<<"recurse"<<endl;
			ww=suf;
		}
	}
	this->rules.insert(xieps);
	this->rules.insert(xitr.begin(),xitr.end());
	this->rules.insert(seqr.begin(),seqr.end());
//cout<<"xdrfinished"<<endl;
	
	//
}

void Grammar::toGreibachNormalForm()
{
	//TODO
	
	this->toChomskyNormalForm();
	
}
string Grammar::testprint()
{
	stringstream ss;
	Word* sigword=new Word(this->start);
	ss<<"Grammar [Start symbol = "<<sigword ->toString()<<endl;
	ss<<"		  N = {";

	for(auto nt : this->nonterminals)
	{
		Word* w=new Word(nt);
		ss<<w ->toString()<<", ";
	}

	ss<<"}"<<endl;
	ss<<"		  T = {";
	for(auto t : this->terminals)
	{
		Word* w=new Word(t);
		ss<<w ->toString()<<", ";
	}

	ss<<"}"<<endl;
	ss<<"		  P = {"<<endl;
	for(auto nt:this->nonterminals)
	{
		Word* w=new Word(nt);
		set<pair<int,Word*>> nr=this->getRulesFromNonterminal(nt);
		if(nr.empty())
		{
			continue;
		}
		ss<<"		       "<<w ->toString()<<" -> ";
		for(auto rule:nr)
		{
			if(rule.second == NULL)
			{
				cout<<"rule ?->NULL!"<<endl;
				exit(1);
			}
			Word* wwww=rule.second;
			ss<<wwww ->toString()<<" | ";
		}
		ss<<endl;
	}

	ss<<"		      }"<<endl<<"        ]";
	return ss.str();
}

void Grammar::printr(pair<int,Word*> r)
{
	if(r.second==NULL)
	{
		cout<<r.first<<" -> NULL!"<<endl;
		exit(1);
	}
	cout<<r.first<<" -> "<<r.second->toString()<<endl;
}
