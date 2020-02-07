#include "PrecedentialRelation.h"

PrecedentialRelation::PrecedentialRelation(Grammar *grammar)
{
    set<char> nonterminals = grammar->getNonterminals();
    set<char> terminals = grammar->getTerminals();
    //Computing FIRST and LAST sets
    set<pair<char, char>> FIRSTPLUS, LASTPLUS;
    for (auto rule : grammar->getRules())
    {
        FIRSTPLUS.insert({rule.first[0], rule.second[0]});
        LASTPLUS.insert({rule.first[0], rule.second[rule.second.size() - 1]});
    }
    //Computing FIRST+
    FIRSTPLUS = this->transitiveClosure(FIRSTPLUS, true, grammar);
    //Computing LAST+
    LASTPLUS = this->transitiveClosure(LASTPLUS, false, grammar);
    //Computing final relations for =
    for (auto rule : grammar->getRules())
        for (int i = 0; i < rule.second.size() - 1; i++)
            this->equalsRelation.insert({rule.second[i], rule.second[i + 1]});

    //Computing final relations for <
    this->lessRelation = this->composition(this->equalsRelation, FIRSTPLUS);

    //Computing FIRST*
    set<pair<char, char>> FIRSTSTAR = FIRSTPLUS;
    for (auto symbol : nonterminals)
        FIRSTSTAR.insert({symbol, symbol});
    for (auto symbol : terminals)
        FIRSTSTAR.insert({symbol, symbol});

    //Computing final relations for >
    set<pair<char, char>> helpRelation = this->inverseRelation(LASTPLUS);
    helpRelation = this->composition(helpRelation, this->equalsRelation);
    helpRelation = this->composition(helpRelation, FIRSTSTAR);

    for (auto element : helpRelation)
        if (terminals.count(element.second))
            this->moreRelation.insert(element);
}

set<pair<char, char>> PrecedentialRelation::composition(set<pair<char, char>> relation1, set<pair<char, char>> relation2) const
{
    set<pair<char, char>> compositeRelation;
    for (auto elementRelation1 : relation1)
        for (auto elementRelaton2 : relation2)
            if (elementRelation1.second == elementRelaton2.first)
                compositeRelation.insert({elementRelation1.first, elementRelaton2.second});

    return compositeRelation;
}

set<pair<char, char>> PrecedentialRelation::transitiveClosure(set<pair<char, char>> relation, bool forFIRST, Grammar *grammar) const
{

    set<char> nonterminals = grammar->getNonterminals();
    set<pair<string, string>> rules = grammar->getRules();
    map<pair<char, char>, bool> usedElements;
    queue<pair<char, char>> chars;
    for (auto element : relation)
    {
        usedElements[element] = true;
        if (nonterminals.count(element.second))
            chars.push(element);
    }

    while (!chars.empty())
    {
        pair<char, char> element = chars.front();
        chars.pop();

        for (auto rule : rules)
        {

            if (rule.first[0] == element.second)
            {
                int index = forFIRST ? 0 : rule.second.size() - 1;
                relation.insert({element.first, rule.second[index]});
                if (usedElements.count({element.first, rule.second[index]}) == 0 && nonterminals.count(rule.second[index] != 0))
                {
                    chars.push({element.first, rule.second[index]});
                    usedElements[{element.first, rule.second[index]}] = true;
                }
            }
        }
    }
    return relation;
}

set<pair<char, char>> PrecedentialRelation::inverseRelation(set<pair<char, char>> relation) const
{
    set<pair<char, char>> inverseRelation;
    for (auto element : relation)
        inverseRelation.insert({element.second, element.first});
    return inverseRelation;
}

bool PrecedentialRelation::isInLessRelation(pair<char, char> element) const
{
    return this->lessRelation.count(element) != 0;
}

bool PrecedentialRelation::isInEqualsRelation(pair<char, char> element) const
{
    return this->equalsRelation.count(element) != 0;
}

bool PrecedentialRelation::isInMoreRelation(pair<char, char> element) const
{
    return this->moreRelation.count(element) != 0;
}

bool PrecedentialRelation::atMostOneRelation() const
{
    for (pair<char, char> element : this->lessRelation)
        if (this->equalsRelation.count(element) != 0 || this->moreRelation.count(element) != 0)
            return false;
    for (pair<char, char> element : this->equalsRelation)
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
