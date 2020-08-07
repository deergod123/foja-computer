#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(Grammar *grammar)
{
    this->grammar = grammar;
}

Word* SyntaxAnalyzer::getCurrentWord(stack<int> stk, Word* w, Symbol* currentSymbol) const
{
    //string currentWord = "";
	cout<<"running getCurrentWord"<<endl;
	cout<<"w: "<<w->toString()<<" s: "<<(currentSymbol == NULL ? 0 : currentSymbol->id)<<" c: "<<w->contains(currentSymbol)<<endl;
	Word* currentWord = new Word();
    while (stk.top() != BOTTOM_OF_STACK)
    {
        int symb = stk.top();
        currentWord->conc(new Word(symb), false);
        stk.pop();
    }
    //reverse(currentWord.begin(), currentWord.end());
	cout<<"cw1: "<<currentWord->toString()<<endl;
	currentWord->reverse();
	cout<<"cw2: "<<currentWord->toString()<<endl;

	/*
    while (currentSymbol != w->end;)
    {
        currentWord->conc(new Word(currentSymbol->id),false);
        currentSymbol=currentSymbol->next;
    }
        */
	if(!(w->isEmpty()))
	{
		currentWord->insert(currentWord->getEnd(),currentSymbol,w->getEnd());
	}
	cout<<"cw: "<< currentWord->toString()<<endl;
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
		cmd->reduceRule=rule;
	    	commands.push_back(cmd);
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
cout<<"running shiftAndReduce"<<endl;
    vector<Command*> commands;
cout<<"building relation"<<endl;
    PrecedentialRelation *precedentialRelation = new PrecedentialRelation(this->grammar);
    if (!precedentialRelation->atMostOneRelation() || w->isEmpty())
    {
	cout<<"REJECT"<<endl;
	Command* cmd=new Command();
	cmd->value=CMD_REJECT;
	cmd->currentWord=NULL;
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
	//
	cout<<"stack top: "<<stk.top()<<endl;
	//
	pair<int, int> par;
        if(currentSymbol!=NULL)
	{
		par = make_pair(stk.top(), currentSymbol->id);
		cout<<"current symbol: "<<currentSymbol -> id<<endl;
	}
	else
	{
		par = make_pair(stk.top(), NOSYMBOL);
		cout<<"current symbol: 0"<<endl;
	}
        bool stop = false;

        if (stk.top() == BOTTOM_OF_STACK)
	{
		cout<<"shift: stack bottom"<<endl;
            this->shift(stk, w, (&currentSymbol), commands);
        }
	else if (precedentialRelation->isInEqualsRelation(par))
        {
		cout<<"shift: equal"<<endl;
	    this->shift(stk, w, (&currentSymbol), commands);
        }
	else if (precedentialRelation->isInLessRelation(par))
        {
		cout<<"shift: less"<<endl;
	    this->shift(stk, w, (&currentSymbol), commands);
        }
	else if (precedentialRelation->isInMoreRelation(par))
        {
		cout<<"reduce: more"<<endl;
	    stop = this->reduce(stk, precedentialRelation, w, currentSymbol, commands);
        }
	else if (currentSymbol == NULL && stk.top() != BOTTOM_OF_STACK)
        {
		cout<<"reduce: no symbol and non-empty stack"<<endl;
	    stop = this->reduce(stk, precedentialRelation, w, currentSymbol, commands);
        }
	else
	{
            break;
	}
        if (stop)
        {
	    break;
	}
    }

    if (this->shiftAndReduceAnalyzed(stk, currentSymbol))
	{
		Command* cmd=new Command();
		cmd->value=CMD_ACCEPT;
		cmd->currentWord=NULL;
       		commands.push_back(cmd);
    	}


    else
	{
		Command* cmd=new Command();
		cmd->value=CMD_REJECT;
		cmd->currentWord=NULL;
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
	Word* w;
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
				cout<<"red"<<endl;
				w=new Word(commands[i]->reduceRule.first);
				ss<<"REDUCE "<< commands[i]->reduceRule.second->toString() << " -> " << w->toString();
				cout<<"red2"<<endl;
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
