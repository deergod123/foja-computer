#include "PrecedentialRelation.h"
PrecedentialRelation::PrecedentialRelation(Grammar *grammar)
{
    set<int> nonterminals = grammar->getNonterminals();
    set<int> terminals = grammar->getTerminals();
    //Computing FIRST and LAST sets
    set<pair<int, int>> FIRSTPLUS, LASTPLUS;
    for (auto rule : grammar->getRules())
    {
        FIRSTPLUS.insert({rule.first, rule.second->getStart()->id});
        LASTPLUS.insert({rule.first, rule.second->getEnd()->id});
    }
    //Computing FIRST+
    FIRSTPLUS = this->transitiveClosure(FIRSTPLUS, true, grammar);
    //Computing LAST+
    LASTPLUS = this->transitiveClosure(LASTPLUS, false, grammar);
    //Computing final relations for =
    for (auto rule : grammar->getRules())
        //for (int i = 0; i < rule.second.size() - 1; i++)
          //  this->equalsRelation.insert({rule.second[i], rule.second[i + 1]});
	for(Symbol* s=rule.second->getStart();s!=NULL;s=s->next)
	{
		if(s->next==NULL)
		{
			break;
		}
		this->equalsRelation.insert({s->id,s->next->id});
	}

    //Computing final relations for <
    this->lessRelation = this->composition(this->equalsRelation, FIRSTPLUS);

    //Computing FIRST*
    set<pair<int, int>> FIRSTSTAR = FIRSTPLUS;
    for (auto symbol : nonterminals)
        FIRSTSTAR.insert({symbol, symbol});
    for (auto symbol : terminals)
        FIRSTSTAR.insert({symbol, symbol});

    //Computing final relations for >
    set<pair<int, int>> helpRelation = this->inverseRelation(LASTPLUS);
    helpRelation = this->composition(helpRelation, this->equalsRelation);
    helpRelation = this->composition(helpRelation, FIRSTSTAR);

    for (auto element : helpRelation)
        if (terminals.count(element.second))
            this->moreRelation.insert(element);
}

set<pair<int, int>> PrecedentialRelation::composition(set<pair<int, int>> relation1, set<pair<int, int>> relation2) const
{
    set<pair<int, int>> compositeRelation;
    for (auto elementRelation1 : relation1)
        for (auto elementRelaton2 : relation2)
            if (elementRelation1.second == elementRelaton2.first)
                compositeRelation.insert({elementRelation1.first, elementRelaton2.second});

    return compositeRelation;
}

set<pair<int, int>> PrecedentialRelation::transitiveClosure(set<pair<int, int>> relation, bool forFIRST, Grammar *grammar) const
{

    set<int> nonterminals = grammar->getNonterminals();
    ruleset_t rules = grammar->getRules();
    map<pair<int, int>, bool> usedElements;
    queue<pair<int, int>> chars;
    for (auto element : relation)
    {
        usedElements[element] = true;
        if (nonterminals.count(element.second))
            chars.push(element);
    }

    while (!chars.empty())
    {
        pair<int, int> element = chars.front();
        chars.pop();

        for (auto rule : rules)
        {
		if(rule.second->isEmpty())
		{
			continue;
		}
            if (rule.first == element.second)
            {
		/*
                int index = forFIRST ? 0 : rule.second.size() - 1;
                relation.insert({element.first, rule.second[index]});
                if (usedElements.count({element.first, rule.second[index]}) == 0 && nonterminals.count(rule.second[index] != 0))
                {
                    chars.push({element.first, rule.second[index]});
                    usedElements[{element.first, rule.second[index]}] = true;
                }
		*/
		int item = forFIRST ? rule.second->getStart()->id : rule.second->getEnd()->id;
		relation.insert({element.first, item});
		if(usedElements.count({element.first, item}) == 0 && nonterminals.count(item)!=0)
		{
			chars.push({element.first, item});
			usedElements[{element.first, item}] = true;
		}
            }
        }
    }
    return relation;
}

set<pair<int, int>> PrecedentialRelation::inverseRelation(set<pair<int, int>> relation) const
{
    set<pair<int, int>> inverseRelation;
    for (auto element : relation)
        inverseRelation.insert({element.second, element.first});
    return inverseRelation;
}

bool PrecedentialRelation::isInLessRelation(pair<int, int> element) const
{
    return this->lessRelation.count(element) != 0;
}

bool PrecedentialRelation::isInEqualsRelation(pair<int, int> element) const
{
    return this->equalsRelation.count(element) != 0;
}

bool PrecedentialRelation::isInMoreRelation(pair<int, int> element) const
{
    return this->moreRelation.count(element) != 0;
}

bool PrecedentialRelation::atMostOneRelation() const
{
    for (pair<int, int> element : this->lessRelation)
        if (this->equalsRelation.count(element) != 0 || this->moreRelation.count(element) != 0)
            return false;
    for (pair<int, int> element : this->equalsRelation)
        if (this->moreRelation.count(element) != 0)
            return false;

    return true;
}

string PrecedentialRelation::getPrecedentialRelationString() const
{
    string stringPrecedentialRelation = "";
    for (auto element : this->lessRelation)
        stringPrecedentialRelation +=
            static_cast<std::stringstream &>(std::stringstream() << element.first << "," << element.second << ";").str();

    if (this->lessRelation.size() != 0)
        stringPrecedentialRelation.pop_back();
    stringPrecedentialRelation += "|";

    for (auto element : this->equalsRelation)
        stringPrecedentialRelation +=
            static_cast<std::stringstream &>(std::stringstream() << element.first << "," << element.second << ";").str();

    if (this->equalsRelation.size() != 0)
        stringPrecedentialRelation.pop_back();
    stringPrecedentialRelation += "|";

    for (auto element : this->moreRelation)
        stringPrecedentialRelation +=
            static_cast<std::stringstream &>(std::stringstream() << element.first << "," << element.second << ";").str();

    if (this->moreRelation.size() != 0)
        stringPrecedentialRelation.pop_back();

    cout << stringPrecedentialRelation << endl;
    return stringPrecedentialRelation;
}

void PrecedentialRelation::print() const
{
    for (auto par : this->lessRelation)
        cout << "(" << par.first << " , " << par.second << ") ";
    cout << endl;
    for (auto par : this->equalsRelation)
        cout << "(" << par.first << " , " << par.second << ") ";
    cout << endl;
    for (auto par : this->moreRelation)
        cout << "(" << par.first << " , " << par.second << ") ";
    cout << endl;
}
