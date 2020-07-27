#include "Grammar.h"
#include "../relations/PrecedentialRelation.h"
#include "../words/Word.h"



rulecomp::rulecomp()
{

}

/*
bool rulecomp::operator() (rule_t r1, rule_t r2) const
{
	wordcomp* wc=new wordcomp();
	return	!(r1==r2 ||(
			(r1.first==r2.first)
			&& (
				!(*wc)(r1.second,r2.second)
				|| !(*wc)(r2.second,r1.second)
			)
		)
		);
}
*/

bool rulecomp::operator() (rule_t r1, rule_t r2) const
{
	//cout<<"rulecomp: "<<r1.first<< "->" << r1.second->toString()<<" and " << r2.first<< "->" << r2.second->toString()<<endl;
	if(r1.first!=r2.first)
	{
		//cout<<"left diff"<<endl;
		return r1.first>r2.first; //used >, left side is a nonterminal, therefore it is negative and ordering should be reversed
	}
	if(r1.second==r2.second)
	{
		//cout<<"right eq pointer"<<endl;
		return false;
	}
	if(r1.second==NULL)
	{
		//cout<<"right r1 NULL"<<endl;
		return false;
	}
	return r1.second->less(r2.second); //comparing right sides
}

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

ruleset_t Grammar::getRules() const
{
    return this->rules;
}
///////////////////////////
///////////////////////////

Grammar::Grammar(set<int> n,set<int> t,ruleset_t p, int s)
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
		rule_t rp;
		//cout<<rule.first<<" -> ";
		rp.first=rule.first;
		//cout<<rule.second->toString()<<endl;
		rp.second=rule.second->clone();
		this->rules.insert(rp);
	}
	//cout<<"complete"<<endl;

}
//


Grammar::Grammar(string stringGrammar)
{
	cout<<"Constructing grammar from "<<stringGrammar<<endl;
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
	Word* w=new Word(segment);
	Symbol* s=w->getStart();
        this->nonterminals.insert(s->id);
    }

    stringstream terminalsStream(grammar[1]);
    while (getline(terminalsStream, segment, ','))
    {
	Word* w=new Word(segment);
cout<<"1"<<endl;
	Symbol* s=w->getStart();
        this->terminals.insert(s->id);

    }
cout<<grammar[2]<<endl;
    Word* w=new Word(grammar[2]);
cout<<"2"<<endl;
	Symbol* s=w->getStart();

    this->start = s->id;

    stringstream rulesStream(grammar[3]);
    rule_t rule;
    rule.first = 0;
    rule.second = new Word();
cout<<"rules"<<endl;
    while (getline(rulesStream, segment, ','))
    {
        if (rule.first == 0)
        {
            //rule.first = segment;
		if(segment.size()==1)
		{
			if(segment[0]>='A' && segment[0]<='Z')
			{
				rule.first=(1+(int)segment[0]-(int)'A')*-1;
				continue;
			}
			cout<<"Rule from terminal?"<<endl;
			exit(1);
		}
		if(segment.size()==0)
		{
			cout<<"Rule from empty string?"<<endl;
			exit(1);
		}
		if(!(segment[0]=='(' && segment[1]=='N' && segment.back()==')'))
		{
			cout<<"Rule from invalid symbol"<<endl;
			exit(1);
		}
		string str=segment.erase(0,2);
		str.pop_back();
		int id;
		stringstream idstream(str);
		idstream >> id;
		rule.first=id*-1;
		continue;
        }
	rule.second = new Word(segment);
        //if (rule.second == substitutedEpsilon)
        //	rule.second = epsilon;
        this->rules.insert(rule);
        rule.first = 0;
        rule.second = new Word();
    }
}

string Grammar::grammarToString()
{
	stringstream sg;
	for(int n:this->nonterminals)
	{
		Word* w=new Word(n);
		sg<< w->toString()<<",";
	}
	//sg.unget();
	sg.seekp(-1,sg.cur);
//cout<<sg.str()<<endl;
	sg<<"|";
        for(int n:this->terminals)
        {
                Word* w=new Word(n);
                sg<< w->toString()<<",";
        }
//cout<<sg.str()<<endl;
	//sg.unget();
	sg.seekp(-1,sg.cur);
	sg<<"|";
	Word* w=new Word(this->start);
	sg<< w->toString()<<"|";
//cout<<sg.str()<<endl;
//cout<<"rules"<<endl;
	for(auto rule:this->rules)
	{
		w=new Word(rule.first);
		sg<< w->toString()<<",";
		sg<< rule.second->toString();
		sg<<",";
//cout<<sg.str()<<endl;
	}
	//sg.unget();
	//sg.seekp(-1,sg.cur);
	string o=sg.str();
	o.pop_back();
	return o;

}




pair<bool,bool> Grammar::patchNonterminals()
{
	pair<bool,bool> o;
	o.first=false; //added nonterminals
	o.second=false; //removed nonterminals
	for(auto rule:this->rules)
	{
		if(rule.first<0)
			o.first=o.first || (this->nonterminals.insert(rule.first).second);
		for(Symbol* s=rule.second->getStart();s!=NULL;s=s->next)
		{
			if(s->id<0)
				o.first=o.first ||(this->nonterminals.insert(s->id).second);
		}
	}
	//
	for(int n:this->nonterminals)
	{
		bool remove=true;
		for(auto rule:this->rules)
		{
			if(n==rule.first)
			{
				remove=false;
				break;
			}
			for(Symbol* s=rule.second->getStart();s!=NULL;s=s->next)
			{
				if(n==s->id)
				{
					remove=false;
					break;
				}
			}
			if(!remove)
				break;
		}
		if(remove)
		{
			o.second=true;
			this->nonterminals.erase(n);
		}
	}
	return o;
}
/*

//this function is deprecated
//replaced by ruleset_t macro
bool Grammar::removeDupedRules()
{
	//cout<<"removing dupes"<<endl;
	bool o=false;
	ruleset_t newrules;
	for(auto n:this->nonterminals)
	{
		//Word* w=new Word(n);
		//cout<<"Nonterminal "<< w->toString()<<endl<<"------------------------"<<endl;
		ruleset_t newrulesn;
		for(auto rule:this->getRulesFromNonterminal(n))
		{
			//this->printr(rule);
			bool add=true;
			for(auto newrule:newrulesn)
			{
				//cout<<"	";
				//this->printr(newrule);
				if(rule.second->equal(newrule.second))
				{
					//cout<<"Dupe found"<<endl;
					add=false;
					o=true;
					break;
				}
			}
			if(add)
			{
				//cout<<"adding"<<endl;
				newrules.insert(rule);
				newrulesn.insert(rule);
			}
		}
	}
	this->rules.clear();
	this->rules.insert(newrules.begin(), newrules.end());
	return o;
}
*/



//no idea how this works, I just rewrote it and added a patchNonterminals @ the end
//TODO add comments, format
void Grammar::toReducedNormalForm()
{

	//A->A
    for (auto rule : this->rules)
        if ((rule.second!=NULL)&&(!rule.second->isEmpty())&&(rule.first == rule.second->getStart()->id ) && (rule.second->getStart()->next == NULL))
            this->rules.erase(rule);

	//

//cout<<"1";

    set<int> terminatedNonterminals;

	bool cont=true;
    while (cont)
    {
	cont=false;
        for (auto rule : this->rules)
        {
            bool terminatedRule = true;
            //
		for(Symbol* s=rule.second->getStart();s!=NULL;s=s->next)
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
		cont=true;
            }
        }
    }

//cout<<"2";
    for (auto rule : this->rules)

    {
        if (terminatedNonterminals.count(rule.first) == 0)
        {
            this->rules.erase(rule);
            continue;
        }
		/*
        for (unsigned int i = 0; i < rule.second.length(); i++)
            if ((terminatedNonterminals.count(rule.second[i]) == 0) && (this->terminals.count(rule.second[i]) == 0))
            {
                this->rules.erase(rule);
                break;
            }
		*/
		for(Symbol* s=rule.second->getStart();s!=NULL;s=s->next)
		{
			//if((terminatedNonterminals.count(s->id)==0) && (this->terminals.count(s->id)==0))
			if((s->id<0)&&(terminatedNonterminals.count(s ->id)==0))
			{
				this->rules.erase(rule);
				break;
			}
		}
    }

    terminatedNonterminals.clear();

//cout<<"3";
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
				/*
                for (unsigned int i = 0; i < rule.second.length(); i++)
                {
                    if ((achievableNonterminals.count(rule.second[i]) == 0) && (this->terminals.count(rule.second[i]) == 0))
                    {
                        achievableNonterminals.insert(rule.second[i]);
                        q.push(rule.second[i]);
                    }
                }
				*/
			for(Symbol* s=rule.second->getStart();s!=NULL;s=s->next)
			{
				//if((achievableNonterminals.count(s->id)==0) && (this->terminals.count(s->id)==0))
				if((s->id<0) && (achievableNonterminals.count(s->id)==0))
				{
					achievableNonterminals.insert(s->id);
					q.push(s->id);
				}
			}
            }
        }
    }
//cout<<"4";
    for (auto rule : this->rules)
    {
        if (achievableNonterminals.count(rule.first) == 0)
        {
            this->rules.erase(rule);
            continue;
        }
		/*
        for (unsigned int i = 0; i < rule.second.length(); i++)
            if ((achievableNonterminals.count(rule.second[i]) == 0) && (this->terminals.count(rule.second[i]) == 0))
            {
                this->rules.erase(rule);
                break;
            }
		*/
		for(Symbol* s=rule.second->getStart();s!=NULL;s=s->next)
		{
			//if((achievableNonterminals.count(s->id)==0)&&(this->terminals.count(s->id)==0))
			if((s->id<0) && (achievableNonterminals.count(s->id)==0))
			{
				this->rules.erase(rule);
				break;
			}
		}
    }

    achievableNonterminals.clear(); //why?
	this->patchNonterminals();
	//this->removeDupedRules(); //depr
}




							/////////////////////////////////////
/////////////////////////////       							////////////////////////////////////////////////
							/////////////////////////////////////



/*
no idea how exactly this works, just reworked it
overengineered, ugly
TODO comment
can't be bothered to rework this completely
commented blocks are old code for reference
*/
void Grammar::toEpsilonFreeForm()
{
    set<int> removableNonterminals;
    for (auto rule : this->rules)
        if (rule.second->isEmpty())
            removableNonterminals.insert(rule.first);

    //finding out removable rules
    while (true) //why not while(!end)???????? also there is a break? what is this?
    {
        bool end = true;
        for (auto rule : this->rules)
        {
            bool removableRule = true; //isn't this unnecessary? breaks after this is changed anyway?
			/*
            for (unsigned int i = 0; i < rule.second.length(); i++)
            {
                if (removableNonterminals.count(rule.second[i]) == 0)
                {
                    removableRule = false;
                    break;
                }
            }
			*/
			for(Symbol* s=rule.second->getStart();s!=NULL;s=s->next)
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
	//W	H	Y	?
        if (end)
            break;
    }

    //computing new rules
    ruleset_t newRules;

    for (auto rule : this->rules)
    {
        string combination = ""; //why is this a string?
		/*
        for (unsigned int i = 0; i < rule.second.length(); i++)
            if (removableNonterminals.count(rule.second[i]) != 0)
                combination += "0";
		*/
		for(Symbol* s=rule.second->getStart();s!=NULL;s=s->next)
		{
			if(removableNonterminals.count(s->id)!=0)
			{
				combination+="0";
			}
		}
        if (combination == "")
            continue;

        while (count(combination.begin(), combination.end(), '1') != combination.length())
        {
            Word* newrule = new Word();
            int removableNonterminal = 0;
			/*
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
			*/
			for(Symbol* s=rule.second->getStart();s!=NULL;s=s->next)
			{
				if(removableNonterminals.count(s->id)!=0)
				{
					if(combination[removableNonterminal]=='1')
					{
						/*
						Symbol* ss=new Symbol();
							ss->prev=NULL;
							ss->next=NULL;
							ss->id=s->id;
						newrule.conc(new Word(ss),false);
						//updated for id constructor
						*/
						newrule->conc(new Word(s->id),false);
					}
					removableNonterminal++;
				}
				else
				{
					/*
					Symbol* ss=new Symbol();
						ss->prev=NULL;
						ss->next=NULL;
						ss->id=s->id;
					newrule.conc(new Word(ss),false);
					//updated
					*/
					newrule->conc(new Word(s->id),false);
				}
			}
            for (int i = combination.length() - 1; i >= 0; i--)
            {
                combination[i] = combination[i] == '0' ? '1' : '0';
                if (combination[i] == '1')
                    break;
            }

            rule_t newRulePair;
            newRulePair.first = rule.first;
            newRulePair.second = newrule;
            newRules.insert(newRulePair);
        }
    }

    //inserting new rules
    for (auto newRule : newRules) //why like this?
        this->rules.insert(newRule);

    //erasing epsilon rules
    for (auto rule : this->rules)
        if (rule.second->isEmpty())
            this->rules.erase(rule);

	//this->removeDupedRules(); //depr
}




int Grammar::countOfTerminals(Word* w)
{
    int count = 0;
	/*
	for (unsigned int i = 0; i < word.length(); i++)
        if ((((int)word[i]) >= 97) && (((int)word[i]) <= 122))
            count++;
    */
	//cout<<"COT for "<<w->toString()<<endl;
	for(Symbol* s=w->getStart();s!=NULL;s=s->next)
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
    for (int i = 0; i < size; i++)
    {
        pair<Word*, int> rule = queue.front();
        queue.pop();
     	cout<<"min q "<<rule.first->toString()<<" "<<rule.second<<endl;
	if (i == 0)
            minimum = rule.second;
        else if (minimum > rule.second)
            minimum = rule.second;
        queue.push(rule);
    }
    return minimum;
}

int Grammar::minimum(wordset_t words)
{
    int minimum = 0, i = 0;
    for (auto word : words)
    {
	cout<<"min ws "<<word->toString()<<" "<<word->length()<<endl;
        if (i == 0)
        {
            minimum = word->length();
            i++;
        }
        else if (minimum > word->length())
            minimum = word->length();
    }
    return minimum;
}









string Grammar::isEquivalent(Grammar *grammar)
{
    int maxMemory = 0;
    int reachedSize = 0;
    queue<pair<Word*, int>> thisQueue, grammarQueue;
    wordset_t thisWords;
wordset_t  grammarWords;
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

//cout<<"for"<<endl;
    for (unsigned int i = 0; i < 10000; i++)
    {
cout<<"iter"<<i<<endl;
        if (i % 1000 == 0)
        {
            int thisMinimum = thisQueue.size() == 0 ? this->minimum(thisWords) : this->minimum(thisQueue);
            int grammarMinimum = grammarQueue.size() == 0 ? this->minimum(grammarWords) : this->minimum(grammarQueue);
            reachedSize = max(reachedSize, min(thisMinimum, grammarMinimum));
            for (auto word : thisWords)
            {
                if ((word->length() < min(thisMinimum, grammarMinimum)) && (grammarWords.count(word) == 0))
                    return word->toString() + "|0";
            }

            for (auto word : grammarWords)
            {
                if ((word->length() < min(thisMinimum, grammarMinimum)) && (thisWords.count(word) == 0))
                    return word->toString() + "|0";
            }
        }

        pair<Word*, int> thisWord;
        if (!thisQueue.empty())
        {
//cout<<"tq not empty"<<endl;
            thisWord = thisQueue.front();
            thisQueue.pop();
        }
	else
	{
		thisWord.first=new Word();
	}

        maxMemory -= thisWord.first->length();

        pair<Word*, int> grammarWord;
        if (!grammarQueue.empty())
        {
//cout<<"gq not empty"<<endl;
            grammarWord = grammarQueue.front();
            grammarQueue.pop();
        }
	else
	{
		grammarWord.first=new Word();
	}

        maxMemory -= grammarWord.first->length();

	/*
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
	*/
cout<<grammarWord.first ->toString()<<endl;
	for(Symbol* s=grammarWord.first->getStart();s!=NULL;s=s->next)
	{
//cout<< s->id<<endl;
		if(s->id>0)
		{
//cout<<"cont"<<endl;
			continue;
		}
//cout<<"notcont"<<endl;
		for(auto rule: grammar->getRules())
		{
//cout<<"sus"<<endl;

			if(rule.first == s->id)
			{
//cout<<"sus2"<<endl;

				pair<Word*, int> newPair= grammarWord;
				newPair.first=newPair.first->clone();

				newPair.first->replace(s, rule.second);
				//cout<<newPair.second<<" -->> ";
				//newPair.second+= grammar->countOfTerminals(rule.second);
				//cout<<newPair.second<<endl;
				newPair.second=countOfTerminals(newPair.first);
				if(newPair.second == newPair.first->length())
					grammarWords.insert(newPair.first);
				else
					grammarQueue.push(newPair);

				if(maxMemory +newPair.first->length() > 50000000)
					return to_string(reachedSize) + "|1";
				else
					maxMemory+=newPair.first->length();
			}
		}
	}
    }

    return to_string(reachedSize) + "|1";
}
/*
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
*/
bool Grammar::isPrecedential()
{
    // there cant be the rule with epsilon
    for (auto rule : this->rules)
        if (rule.second->isEmpty())
            return false;

    // there cant be more than one rules with the same right side
    /*
	map<string, int> amountOfRules;
    for (auto rule : this->rules)
    {
        amountOfRules[rule.second]++;
        if (amountOfRules[rule.second] > 1)
            return false;
    }
	*/
	for(auto rule:this->rules)
	{
		for(auto rule2:this->rules)
		{
			if((rule!=rule2) && (rule.second->equal(rule2.second)))
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
string Grammar::hello() const
{
    return "hello world!";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////EDIT









ruleset_t Grammar::getRulesFromNonterminal(int nt)
{
	if(!(this->nonterminals.count(nt)))
	{
		cout << "Grammar does not contain nonterminal id " << nt <<endl;
	}
	ruleset_t o;
	for(auto rule:this->rules)
	{
		if(rule.first==nt)
		{
			rule_t np;
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
	/*find the highest nonterminal slot used
	the next N will be used for creating new
	stored as int nextnewnt

	After a new N is created, nextnewnt-- */

	int nextnewnt=-1;

	for(int n:this->nonterminals)
	{
		if(n<=nextnewnt)
		{
			nextnewnt=n-1;
		}
	}

	/*for each T create new rule N->T,
	stored as xitr */
	ruleset_t xitr;

	for(int t:this->terminals)
	{
		rule_t nxitrp;
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

	/*swap terminals for their corresponding nonterminal
	ignore N->T and N->eps, as they are valid rules anyway
	*/

	for(auto rule:this->rules)
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
		for(;s!=NULL;s=s->next) //anything else
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

	/*
	create new rule N->eps and a word containing
	only N in preparation for the next step
	*/

	rule_t xieps; //N->eps
	xieps.first=nextnewnt;
	nextnewnt--;
	this->nonterminals.insert(xieps.first);
	xieps.second=new Word();

	//
	//Symbol* es=new Symbol();
	//es->prev=NULL;
	//es->next=NULL;
	//es->id=xieps.first;
	//updated for id-specific constructor


	Word* xiepsw=new Word(xieps.first); //word containing only xi_eps (for conc)
	/*
	deconstruct rules with right side longer than 3 N
	into a sequence of valid rules, store as seqr
	*/

	ruleset_t seqr;

	for(auto rule:this->rules)
	{
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

		assert(s->id <0); //we should not encounter Ts from here on

		if(s->next==NULL) //N->N chain rules, xiepsw must be added
		{
			rule.second->conc(xiepsw,true);
			seqr.insert(rule);
			continue;
		}

		assert(s->next->id<0);

		if(s->next->next==NULL) //N->NN
		{
			seqr.insert(rule);
			continue;
		}
		/*
		this is where the actual deconstruction begins
		*/

		int lastnt=rule.first; //N used for splitting in the previous iteration
		for(Word* ww=rule.second; ; ) //ww is the right side of the rule to be deconstructed
		{

			assert(ww->getStart()->id<0);

			Word* suf=ww->split(ww->getStart()); //separate the first character
			rule_t nseqrp; //new sequence rule
			nseqrp.first=lastnt;
			nseqrp.second=ww;
			//Symbol* ss=new Symbol();
			//ss->prev=NULL;
			//ss->next=NULL;
			//ss->id=nextnewnt;
			//lastnt=nextnewnt;
			//nextnewnt--;
			//updated
			ww->conc(new Word(nextnewnt),false); //add new N for splitting
			this->nonterminals.insert(nextnewnt);   //add N to grammar
			lastnt=nextnewnt; //update for next iteration
			nextnewnt--;
			seqr.insert(nseqrp); //add rule

			assert(suf->length()>=2); //suffix should be at least 2 characters long, should break when it's 2
			//iteration
			if(suf->length()==2) //check whether suf is a valid right side -> break
			{
				assert(suf->getStart()->id<0);
				assert(suf->getEnd()->id<0);

				pair<int,Word* > eseqrp; //sequence end rule
				eseqrp.first=lastnt;
				eseqrp.second=suf;
				seqr.insert(eseqrp);
				break;
			}
			ww=suf; //continue on suffix
		}
	}

	/*
	inserting newly created rules
	*/

	cout<<"rules:"<<endl;
	for(auto rule:this->rules)
	{
		cout<<"  "<<rule.first<< "->" <<rule.second -> toString()<<endl;
	}
	cout<<"xitr:"<<endl;
	for(auto rule:xitr)
	{
		cout<<"  "<<rule.first<< "->" <<rule.second -> toString()<<endl;
		cout<< this->rules.insert(rule).second<<endl;
	}
	cout<<"seqr:"<<endl;
	for(auto rule:seqr)
	{
		cout<<"  "<<rule.first<< "->" <<rule.second -> toString()<<endl;
		cout<<rule.second->length()<<endl;
		cout<< this->rules.count(rule);
		cout<< this->rules.insert(rule).second<<endl;
	}
	/*end tests*/
	this->rules.insert(xieps);
	//this->rules.insert(xitr.begin(),xitr.end()); //does not work? no idea why tho
	//this->rules.insert(seqr.begin(),seqr.end());
	//test 2
		cout<<"all rules:"<<endl;
		for(auto rule:this->rules)
		{
			cout<<"  "<<rule.first<< "->" <<rule.second -> toString()<<endl;
			cout<<this->rules.count(rule)<<endl;
		}

	//this->removeDupedRules(); //depr
}

void Grammar::toGreibachNormalForm()
{
	int nextnewnt=0;
	for(int i:this->nonterminals)
	{
		if(i<=nextnewnt)
		{
			nextnewnt=i-1;
		}
	}
	rule_t xisr; //for replacing S on the right side\
	int xis=nextnewnt;
	nextnewnt--;
	xisr.first=xis;
	xisr.second=new Word(this->start);
	for(auto rule:this->rules)
	{
		for(Symbol* s=rule.second->getStart();s!=NULL;s=s->next)
		{
			if(s->id==this->start)
			{
				s->id=xis;
			}
		}
	}
	this->rules.insert(xisr);
	//check if <EPS> in L(G)
	set<int> nullprod;
	bool change=true;

	while(change && (!nullprod.count(this->start)))
	{
		change=false;
		for(auto n:this->nonterminals)
		{
			for(auto rule:this->getRulesFromNonterminal(n))
			{
				bool npr=true;
				for(Symbol* s=rule.second->getStart();s!=NULL;s=s->next)
				{
					if(!(nullprod.count(s->id)))
					{
						npr=false;
						break;
					}
				}
				if(npr)
				{
					change=change || nullprod.insert(n).second;
					break;
				}
			}
		}
	}
	this->toEpsilonFreeForm();
	if(nullprod.count(this->start))
	{
		rule_t ser; //S-><EPS>
		ser.first=this->start;
		ser.second=new Word();
	}
	//remove unit production
	while(true)
	{
		bool hasunit=false;
		map<int,ruleset_t> unitrules;
		for(auto rule:this->rules)
		{
			if(rule.second->length()==1)
			{
				if(rule.second->getStart()->id<0)
				{
					hasunit=true;
					if(unitrules.count(rule.first))
					{
						unitrules.find(rule.first).second.insert(rule);
						continue;
					}
					ruleset_t rrr;
					rrr.insert(rule);
					unitrules.insert(pair<int,ruleset_t>(rule.first,rule));
				}
			}
		}
		if(!hasunit)
		{
			break;
		}
		for(auto rr:unitrules)
		{
			for(auto rrr:rr.second)
			{
				this->rules.erase(rr);
			}
		}
		ruleset_t addedrules;
		ruleset_t removedrules;
		for(auto rule:this->rules)
		{
			ruleset_t caddrules;
			bool clear=true;
			for(Symbol* s=rule.second->getStart();s!=NULL;s=s->next)
			{
				if(unitrules.count(s->id))
				{
					clear=false;
					caddrules.insert(rule);
					removedrules.insert(rule);
					break;
				}
			}
			while(!clear)
			{
				clear=true;
				for(auto cr:caddrules)
				{
					for(Symbol* s=cr.second->getStart();s!=NULL;s=s->next)
					{
						if(unitrules.count(s->id))
						{
							Word* w =cr.second;
							Symbol* sp=s->prev;
							cr.second=w->clone();
							clear=false;
							for(auto rr:unitrules.find(s->id).second)
							{
								rule_t nrule;
								nrule.first=cr.first;
								w->replace(s,rr.second);
								nrule.second=w->clone();
								if(sp!=NULL)
								{
									w->replace(sp->next,s);
								}
								else
								{
									w->replace(w->getStart(),s);
								}
								caddrules.insert(nrule);
							}
							caddrules.remove(cr);
							break;
						}
					}
					if(!clear)
					{
						break;
					}
				}
			}
			for(auto cr:caddrules)
			{
				addedrules.insert(cr);
			}
		}
		for(auto ar:addedrules)
		{
			this->rules.insert(ar);
		}
		for(auto rr:removedrules)
		{
			this->rules.erase(rr);
		}
	}
////////remove left recursion



/*
	////remove null production
	set<int> nullprod; //Ns such that N->eps exists
	ruleset_t nullrules; //null production rules, will be removed later
	for(auto n:this->nonterminals)
	{
		for(auto rule:this->getRulesFromNonterminal(n))
		{
			if(rule.second->isEmpty())
			{
				nullprod.insert(n);
				nullrules.insert(rule);
				break;
			}
		}
	}
	bool hasnull=true;
	while(hasnull)
	{
		hasnull=false;
		ruleset_t addrules;
		for(auto rule:this->rules)
		{
			for(Symbol* s=rule.second->getStart();s!=NULL;s=s->next)
			{
				if(nullprod.count(s->id))
				{
					hasnull=true;
					nullrules.insert(rule);
					Word* w=rule.second;			//right side to be edited
					rule.second=rule.second.clone();	//original right side is changed into its clone
										//in order to keep s as a valid symbol pointer for editing
										//without breaking the original rule
										//or linking its right side to a different rule
					for(auto nr:this->getRulesFromNonterminal(s->id))	//apply all rules  (s->id) -> *
					{
						Symbol* sn=s->next;		//store s status
						Symbol* sp=s->prev;
						w->replace(s,nr.second);	//apply nr to s
						rule_t addrule;			//rule to be added
						addrule.first=rule.first;
						addrule.second=w.clone();
						addrules.insert(addrule);
						//reset w by re-inserting s and removing applied rule effects
						if(sp==NULL)	//s was the first symbol
						{
							s->prev=NULL;
							s->next=NULL;
							w->insert(NULL,new Word(s),false); 	//reinsert s at start
							s->next=sn;
							sn->prev=s;				//overwrite effects by relinking s
						}
						else if(sn==NULL)	//s was the last symbol
						{
							s->prev=NULL;
							s->next=NULL;
							w->insert(w->getEnd(),new Word(s),false);
							s->prev=sp;
							sp->next=s;
						}
						else
						{
							s->prev=NULL;
							s->next=NULL;
							w->insert(sp,new Word(s),false);
							s->next=sn;
							sn->prev=s;
						}
					}
					break;
				}
			}
		}
		for(auto rule:nullrules)
		{
			this->rules.erase(rule);
		}
		nullrules.clear();

	}

*/
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
		ruleset_t nr=this->getRulesFromNonterminal(nt);
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

void Grammar::printr(rule_t r)
{
	if(r.second==NULL)
	{
		cout<<r.first<<" -> NULL!"<<endl;
		exit(1);
	}
	cout<<r.first<<" -> "<<r.second->toString()<<endl;
}
