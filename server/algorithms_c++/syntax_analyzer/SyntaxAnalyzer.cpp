#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(Grammar *grammar)
{
    this->grammar = grammar;
}

Word* SyntaxAnalyzer::getCurrentWord(stack<int> stk, Word* w, Symbol* currentSymbol) const
{
    //string currentWord = "";
	Word* currentWord = new Word();
    while (stk.top() != BOTTOM_OF_STACK)
    {
        int symb = stk.top();
        currentWord->conc(new Word(symb), false);
        stk.pop();
    }
    //reverse(currentWord.begin(), currentWord.end());
	currentWord->reverse();
	/*
    while (currentSymbol != w->end;)
    {
        currentWord->conc(new Word(currentSymbol->id),false);
        currentSymbol=currentSymbol->next;
    }
        */
	if(!(w->isEmpty()))
	{
		currentWord->insert(currentWord->getEnd(),currentSymbol,w->getEnd()->prev);
	}
    return currentWord;
}

void SyntaxAnalyzer::shift(stack<int> &stk,Word* &w, Symbol** currentSymbol, vector<Command*> &commands)
{
    stk.push((*currentSymbol)->id);
    (*currentSymbol)=((*currentSymbol)->next);
	/*
    commands.push_back("SHIFT Current word: " +
                       this->getCurrentWord(stk, word, pointerOnCurrentChar));
	*/
	Command* cmd=new Command();
	cmd->value=CMD_SHIFT;
	cmd->currentWord=this->getCurrentWord(stk, w, *currentSymbol);
	cmd->reduceRule=NULL;
	commands.push_back(cmd);
}

bool SyntaxAnalyzer::reduce(stack<int> &stk, PrecedentialRelation* &precedentialRelation, Word* &w, Symbol* &currentSymbol, vector<Command*> &commands)
{
    bool stop = true;
    //get word from stack
    //string reducedWord = "";
    	Word* reducedWord = new Word();
    while (stk.top() != BOTTOM_OF_STACK)
    {
        int sid = stk.top();
        reducedWord->conc(new Word(sid),false);
        stk.pop();
        pair<int, int> par = make_pair(stk.top(), sid);
        if (precedentialRelation->isInLessRelation(par))
            break;
    }
    //reverse(reducedWord.begin(), reducedWord.end());
	reducedWord->reverse();
    //find left part of the rule for reducedWord
    for (auto rule : this->grammar->getRules())
        if (rule.second->equal(reducedWord))
        {
            //reducedWord = rule.first; //why?
//																						/////////
            //return reducedWord to the stack //rule.first?
	/*
            for (int i = 0; i < reducedWord.size(); i++)
                stk.push(reducedWord[i]);
       */
	    stk.push(rule.first);
	    //commands.push_back("REDUCE " + rule.second + " -> " + rule.first +
              //                 " Current word: " + this->getCurrentWord(stk, word, pointerOnCurrentChar));
            Command* cmd=new Command();
		cmd->value=CMD_REDUCE;
		cmd->currentWord=this->getCurrentWord(stk,w,currentSymbol);
		cmd->reduceRule=&rule;
	    //reduce is executed so we cant stop and reject syntax analysis
            stop = false;
            break;
        }

    return stop;
}

bool SyntaxAnalyzer::shiftAndReduceAnalyzed(stack<int> &stk, Symbol* &s)
{
    return s == NULL && stk.top() == this->grammar->getStart() && stk.size() == 2;
}

vector<Command*> SyntaxAnalyzer::shiftAndReduce(Word* w)
{
    //initialization needed structures

    vector<Command*> commands;
    PrecedentialRelation *precedentialRelation = new PrecedentialRelation(this->grammar);
    if (!precedentialRelation->atMostOneRelation() || w->isEmpty())
    {
	Command* cmd=new Command();
	cmd->value=CMD_REJECT;
	cmd->currentWord=NULL;
	cmd->reduceRule=NULL;
        commands.push_back(cmd);
        return commands;
    }

    stack<int> stk;
    Symbol* currentSymbol=w->getStart();
    //word.push_back(END_OF_WORD);
    stk.push(BOTTOM_OF_STACK);

    //executing of the algorithm
    while (!this->shiftAndReduceAnalyzed(stk, currentSymbol))
    {
        pair<int, int> par = make_pair(stk.top(), currentSymbol->id);
        bool stop = false;

        if (stk.top() == BOTTOM_OF_STACK)
            this->shift(stk, w, (&currentSymbol), commands);
        else if (precedentialRelation->isInEqualsRelation(par))
            this->shift(stk, w, (&currentSymbol), commands);
        else if (precedentialRelation->isInLessRelation(par))
            this->shift(stk, w, (&currentSymbol), commands);
        else if (precedentialRelation->isInMoreRelation(par))
            stop = this->reduce(stk, precedentialRelation, w, currentSymbol, commands);
        else if (currentSymbol == NULL && stk.top() != BOTTOM_OF_STACK)
            stop = this->reduce(stk, precedentialRelation, w, currentSymbol, commands);
        else
            break;

        if (stop)
            break;
    }

    if (this->shiftAndReduceAnalyzed(stk, currentSymbol))
	{
		Command* cmd=new Command();
		cmd->value=CMD_ACCEPT;
		cmd->currentWord=NULL;
		cmd->reduceRule=NULL;
       		commands.push_back(cmd);
    	}


    else
	{
		Command* cmd=new Command();
		cmd->value=CMD_REJECT;
		cmd->currentWord=NULL;
		cmd->reduceRule=NULL;
       		commands.push_back(cmd);
    	}

    return commands;
}

string SyntaxAnalyzer::getShiftAndReduceString(vector<Command*> commands, string separator)
{
/*
    string shiftAndReduceString = "";
    for (int i = 0; i < commands.size(); i++)
    {
        shiftAndReduceString += commands[i];
        shiftAndReduceString += "|";
    }

    shiftAndReduceString.pop_back();
    return shiftAndReduceString;
*/
	stringstream ss;
	for(int i=0;i<commands.size();i++)
	{
		switch(commands[i]->value) {
			case CMD_ACCEPT:
				ss<<"ACCEPT";
				break;
			case CMD_REJECT:
				ss<<"REJECT";
				break;
			case CMD_SHIFT:
				ss<<"SHIFT Current word: "<< commands[i]->currentWord->toString();
				break;
			case CMD_REDUCE:
				ss<<"REDUCE "<< commands[i]->reduceRule->second->toString() << " -> " << commands[i]->reduceRule->first;
				ss<<" Current word: "<< commands[i]->currentWord->toString();
				break;
			default:
				ss<<"Unknown command";
		}
		ss<<separator;
	}
	string o=ss.str();
	o.pop_back();
	return o;
}

void SyntaxAnalyzer::printCommands(vector<Command*> commands)
{
/*
    for (int i = 0; i < commands.size(); i++)
        cout << commands[i] << endl;
    cout << "END" << endl;
    cout << endl;
*/
cout << this->getShiftAndReduceString(commands,"\n") << "END" <<endl;
}
