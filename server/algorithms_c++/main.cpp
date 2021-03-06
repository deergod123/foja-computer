#include <node.h>
#include <bits/stdc++.h>
#include "grammars/Grammar.h"
#include "relations/PrecedentialRelation.h"
#include "syntax_analyzer/SyntaxAnalyzer.h"

namespace demo
{

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::NewStringType;
using v8::Object;
using v8::String;
using v8::Value;

void Execute(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  if (args.Length() == 0)
  {

    args.GetReturnValue().Set(String::NewFromUtf8(
                                  isolate, "Wrong inputs", NewStringType::kNormal)
                                  .ToLocalChecked());
    return;
  }

  String::Utf8Value argsZero(isolate, args[0]);
  string function(*argsZero);

  if (function == "reducedNormalForm")
  {
    String::Utf8Value str(isolate, args[1]);
    string grammarString(*str);

    Grammar *grammar = new Grammar(grammarString);
	cout<<"running RNF"<<endl;
    grammar->toReducedNormalForm();
    string result = grammar->grammarToString();
   cout<<grammar->testprint()<<endl;
	 delete grammar;
	cout<<"result: "<<result<<endl;
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, result.c_str()).ToLocalChecked());
  }
  else if (function == "epsilonFreeForm")
  {
    String::Utf8Value str(isolate, args[1]);
    string grammarString(*str);

    Grammar *grammar = new Grammar(grammarString);
	cout<<"running EFF"<<endl;
    grammar->toEpsilonFreeForm();
    string result = grammar->grammarToString();
   cout<<grammar->testprint()<<endl;

    delete grammar;
	cout<<"result: "<<result<<endl;

    args.GetReturnValue().Set(String::NewFromUtf8(isolate, result.c_str()).ToLocalChecked());
  }
  else if (function == "equivalence")
  {
    String::Utf8Value str1(isolate, args[1]);
    string grammarString1(*str1);
    String::Utf8Value str2(isolate, args[2]);
    string grammarString2(*str2);
    Grammar *grammar1 = new Grammar(grammarString1);
    Grammar *grammar2 = new Grammar(grammarString2);

	cout<<"running isEquivalent"<<endl;
    string isEquivalent = grammar1->isEquivalent(grammar2);
	cout<<"result: "<<isEquivalent<<endl;

    delete grammar1;
    delete grammar2;
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, isEquivalent.c_str()).ToLocalChecked());
  }
  else if (function == "isPrecedential")
  {
    String::Utf8Value str(isolate, args[1]);
    string grammarString(*str);
    Grammar *grammar = new Grammar(grammarString);
    PrecedentialRelation *precedentialRelation = new PrecedentialRelation(grammar);
	cout<<"running isPrecedential"<<endl;
    string result = "";
    bool isPrecedential = grammar->isPrecedential();

    if (isPrecedential)
      result += "1#";
    else
      result += "0#null";

    if (isPrecedential == false)
    {
      delete grammar;
      delete precedentialRelation;
      args.GetReturnValue().Set(String::NewFromUtf8(isolate, result.c_str()).ToLocalChecked());
      return;
    }

    result += precedentialRelation->getPrecedentialRelationString();

    delete grammar;
    delete precedentialRelation;
	cout<<"result: "<<result<<endl;

    args.GetReturnValue().Set(String::NewFromUtf8(isolate, result.c_str()).ToLocalChecked());
  }
  else if (function == "getWordSyntaxAnalysis")
  {
    String::Utf8Value str1(isolate, args[1]);
    string grammarString(*str1);
    Grammar *grammar = new Grammar(grammarString);

    String::Utf8Value str2(isolate, args[2]);
    string word(*str2);
	cout<<"running syntax analyzer"<<endl;
    SyntaxAnalyzer *syntaxAnalyzer = new SyntaxAnalyzer(grammar);
    string syntaxAnalysis = syntaxAnalyzer->getShiftAndReduceString(syntaxAnalyzer->shiftAndReduce(new Word(word)));

    delete grammar;
    delete syntaxAnalyzer;
	cout<<"result: "<<syntaxAnalysis<<endl;

    args.GetReturnValue().Set(String::NewFromUtf8(isolate, syntaxAnalysis.c_str()).ToLocalChecked());
  }
  else if (function == "ChomskyNormalForm")
  {
    String::Utf8Value str(isolate, args[1]);
    string grammarString(*str);

    Grammar *grammar = new Grammar(grammarString);
   cout<<grammar->testprint()<<endl;

	cout<<"running CNF"<<endl;
    grammar->toChomskyNormalForm();
   cout<<grammar->testprint()<<endl;

    string result = grammar->grammarToString();
    delete grammar;
	cout<<"result: "<<result<<endl;

    args.GetReturnValue().Set(String::NewFromUtf8(isolate, result.c_str()).ToLocalChecked());
  }
  else if (function == "GreibachNormalForm")
  {
    String::Utf8Value str(isolate, args[1]);
    string grammarString(*str);

    Grammar *grammar = new Grammar(grammarString);
   cout<<grammar->testprint()<<endl;

	cout<<"running GNF"<<endl;
    grammar->toGreibachNormalForm();
   cout<<grammar->testprint()<<endl;

    string result = grammar->grammarToString();
    delete grammar;
	cout<<"result: "<<result<<endl;

    args.GetReturnValue().Set(String::NewFromUtf8(isolate, result.c_str()).ToLocalChecked());
  }


  else
  {
	cout<<"undefined function"<<endl;
    args.GetReturnValue().Set(String::NewFromUtf8(
                                  isolate, "Undefined function", NewStringType::kNormal)
                                  .ToLocalChecked());
  }
}

void Initialize(Local<Object> exports)
{
  NODE_SET_METHOD(exports, "execute", Execute);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

} // namespace demo
