#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(Grammar *grammar)
{
    this->grammar = grammar;
}

string SyntaxAnalyzer::getCurrentWord(stack<char> stackOfChars, const string word, int pointerOnCurrentChar) const
{
    string currentWord = "";
    while (stackOfChars.top() != BOTTOM_OF_STACK)
    {
        char character = stackOfChars.top();
        currentWord.push_back(character);
        stackOfChars.pop();
    }

    reverse(currentWord.begin(), currentWord.end());

    while (pointerOnCurrentChar != word.length() - 1)
    {
        currentWord.push_back(word[pointerOnCurrentChar]);
        pointerOnCurrentChar++;
    }

    return currentWord;
}

void SyntaxAnalyzer::shift(stack<char> &stackOfChars, const string &word, int &pointerOnCurrentChar, vector<string> &commands)
{
    stackOfChars.push(word[pointerOnCurrentChar]);
    pointerOnCurrentChar++;
    commands.push_back("SHIFT Current word: " +
                       this->getCurrentWord(stackOfChars, word, pointerOnCurrentChar));
}

bool SyntaxAnalyzer::reduce(stack<char> &stackOfChars, PrecedentialRelation *precedentialRelation, const string &word, const int &pointerOnCurrentChar, vector<string> &commands)
{
    bool stop = true;
    //get word from stack
    string reducedWord = "";
    while (stackOfChars.top() != BOTTOM_OF_STACK)
    {
        char character = stackOfChars.top();
        reducedWord.push_back(character);
        stackOfChars.pop();
        pair<char, char> par = make_pair(stackOfChars.top(), character);
        if (precedentialRelation->isInLessRelation(par))
            break;
    }
    reverse(reducedWord.begin(), reducedWord.end());

    //find left part of the rule for reducedWord
    for (auto rule : this->grammar->getRules())
        if (rule.second == reducedWord)
        {
            reducedWord = rule.first;
            //return reducedWord to the stack
            for (int i = 0; i < reducedWord.size(); i++)
                stackOfChars.push(reducedWord[i]);
            commands.push_back("REDUCE " + rule.second + " -> " + rule.first +
                               " Current word: " + this->getCurrentWord(stackOfChars, word, pointerOnCurrentChar));
            //reduce is executed so we cant stop and reject syntax analysis
            stop = false;
            break;
        }

    return stop;
}

bool SyntaxAnalyzer::shiftAndReduceAnalyzed(stack<char> &stackOfChars, const string &word, int &pointerOnCurrentChar)
{
    return word[pointerOnCurrentChar] == END_OF_WORD && stackOfChars.top() == this->grammar->getStart() && stackOfChars.size() == 2;
}

vector<string> SyntaxAnalyzer::shiftAndReduce(string word)
{
    //initialization needed structures

    vector<string> commands;
    PrecedentialRelation *precedentialRelation = new PrecedentialRelation(this->grammar);
    if (!precedentialRelation->atMostOneRelation() || word == epsilon)
    {

        commands.push_back("REJECT");
        return commands;
    }

    stack<char> stackOfChars;
    int pointerOnCurrentChar = 0;
    word.push_back(END_OF_WORD);
    stackOfChars.push(BOTTOM_OF_STACK);

    //executing of the algorithm
    while (!this->shiftAndReduceAnalyzed(stackOfChars, word, pointerOnCurrentChar))
    {
        pair<char, char> par = make_pair(stackOfChars.top(), word[pointerOnCurrentChar]);
        bool stop = false;

        if (stackOfChars.top() == BOTTOM_OF_STACK)
            this->shift(stackOfChars, word, pointerOnCurrentChar, commands);
        else if (precedentialRelation->isInEqualsRelation(par))
            this->shift(stackOfChars, word, pointerOnCurrentChar, commands);
        else if (precedentialRelation->isInLessRelation(par))
            this->shift(stackOfChars, word, pointerOnCurrentChar, commands);
        else if (precedentialRelation->isInMoreRelation(par))
            stop = this->reduce(stackOfChars, precedentialRelation, word, pointerOnCurrentChar, commands);
        else if (word[pointerOnCurrentChar] == END_OF_WORD && stackOfChars.top() != BOTTOM_OF_STACK)
            stop = this->reduce(stackOfChars, precedentialRelation, word, pointerOnCurrentChar, commands);
        else
            break;

        if (stop)
            break;
    }

    if (this->shiftAndReduceAnalyzed(stackOfChars, word, pointerOnCurrentChar))
        commands.push_back("ACCEPT");
    else
        commands.push_back("REJECT");

    return commands;
}

string SyntaxAnalyzer::getShiftAndReduceString(vector<string> commands)
{
    string shiftAndReduceString = "";
    for (int i = 0; i < commands.size(); i++)
    {
        shiftAndReduceString += commands[i];
        shiftAndReduceString += "|";
    }

    shiftAndReduceString.pop_back();
    return shiftAndReduceString;
}

void SyntaxAnalyzer::printCommands(vector<string> commands) const
{
    for (int i = 0; i < commands.size(); i++)
        cout << commands[i] << endl;
    cout << "END" << endl;
    cout << endl;
}