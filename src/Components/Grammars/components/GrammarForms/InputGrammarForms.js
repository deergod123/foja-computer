import React, { Component } from "react";
import ReducedFormButton from "./Buttons/ReducedFormButton";
import EpsilonFreeButton from "./Buttons/EpsilonFreeButton";
import ChomskyButton from "./Buttons/ChomskyButton";

import "./InputGrammarForms.css";

class InputGrammarForms extends Component {
  constructor(props) {
    super(props);
    this.state = {
      nonterminals: "",
      terminals: "",
      rules: "",
      start: "",
      inputMessage: ""
    };
  }

  handleNonterminalsChange = event => {
    this.setState({ nonterminals: event.target.value });
  };

  handleTerminalsChange = event => {
    this.setState({ terminals: event.target.value });
  };

  handleRulesChange = event => {
    this.setState({ rules: event.target.value });
  };

  handleStartChange = event => {
    this.setState({ start: event.target.value });
  };

  handleInputMessageChange = value => {
    this.setState({ inputMessage: value });
  };

  badLetterProblemTerminals = word => {

    if (word.length === 0) return true;
    if (word.length >1)
    {

	if(word.charAt(0)!=`(` || word.charAt(1)!=`T` || word.charAt(word.length -1) !=`)`)
		return true;
	for(let i=2;i <= word.length-2;i++)
	{
		if(word.charCodeAt(i) < 48 || word.charCodeAt(i) > 57)
			return true;
	}



	return false;
    }
    return word.charCodeAt(0) < 97 || word.charCodeAt(0) > 122;
  };

  badLetterProblemNonterminals = word => {
    if (word.length === 0) return true;
    if (word.length >1)
    {
	if(word.charAt(0)!=`(` || word.charAt(1)!=`N` || word.charAt(word.length -1) !=`)`)
		return true;
	for(let i=2;i <= word.length-2;i++)
	{
		if(word.charCodeAt(i) < 48 || word.charCodeAt(i) > 57)
			return true;
	}

	return false;
    }
    return word.charCodeAt(0) < 65 || word.charCodeAt(0) > 90;
  };

  badRuleWord = (word, nonterminalsSet, terminalsSet) => {
	console.log("checking "+word)
	let high_read=false;
	let high_terminate=false;
	let high_nums=false
	let high_check="";
	for(let i=0;i<word.length;i++)
	{
		console.log(i);
		if(word.charAt(i) === "(")
		{
			console.log("started high read");
			if(high_read)
			{
				console.log("nested high read!!");
				return true;
			}
			high_read=true;
			high_terminate=false;
			high_check="(";
			continue;
		}
		if(word.charAt(i) === ")")
		{
			console.log("ending high read");
			if(!high_terminate)
			{
				console.log("cannot end high read!!");
				return true;
			}
			high_read=false;
			high_nums=false;
			high_check+=")";
			if(!(nonterminalsSet.includes(high_check)  || terminalsSet.includes(high_check)))
			{
				console.log("check failed for \""+high_check+"\"!!");
				return true;
			}
			high_check="";
			continue;
		}
		if(high_read)
		{
			console.log("in high read");
			if(high_nums)
			{
				console.log("reading id");
				if(word.charCodeAt(i) < 48 || word.charCodeAt(i) > 57)
				{
					console.log("id not a number!!");
					return true;
				}
				high_check+=word.charAt(i);
				high_terminate=true;
				continue;
			}
			console.log("N or T");
			if(!(word.charAt(i)==="N" || word.charAt(i)==="T"))
			{
				console.log("Not N or T!!");
				return true;
			}
			high_check+=word.charAt(i);
			high_nums=true;
			continue;
		}
		console.log("low read");
		if(!(nonterminalsSet.includes(word.charAt(i)) || terminalsSet.includes(word.charAt(i))))
		{
			console.log("check failed for "+word.charAt(i)+"!!");
			return true;
		}
	}



    return false;
  };

  isEpsilon = word => {
    return (word.length === 0 || word==="<EPS>");
  };

  readInput = () => {
    console.log(this.props);
    this.handleInputMessageChange("");

    if (
      this.state.nonterminals === "" ||
      this.state.terminals === "" ||
      this.state.rules === ""
    ) {
      this.handleInputMessageChange("There can not be empty sets!");
      return;
    }

    const nonterminals = this.state.nonterminals.split(",");
    const terminals = this.state.terminals.split(",");
    const start = this.state.start;
    const rules = this.state.rules.split('\n');

    const nonterminalsSet = [];
    for (let letter of nonterminals) {
      if (this.badLetterProblemNonterminals(letter)) {
        this.handleInputMessageChange("Bad nonterminals!");
        return;
      }
      nonterminalsSet.push(letter);
    }

    const terminalsSet = [];
    for (let letter of terminals) {
      if (this.badLetterProblemTerminals(letter)) {
        this.handleInputMessageChange("Bad terminals!");
        return;
      }
      terminalsSet.push(letter);
    }

    if (this.state.start === "") {
      this.handleInputMessageChange("Starting nonterminal can not be empty!");
      return;
    }
    const rulesSet = [];

    let nonterminal;
	console.log(rules)
    for (let i = 0; i < rules.length; i += 1) {
	let rulesLine=rules[i].split("->");
	console.log("rl"+i);
	console.log(rulesLine);
          if (
            this.badLetterProblemNonterminals(rulesLine[0]) ||
            !nonterminalsSet.includes(rulesLine[0])
          ) {
            this.handleInputMessageChange("Bad left part of the rule!");
            return;
          }
          nonterminal = rulesLine[0];
          for (let word of rulesLine[1].split("|")) {
            if (this.badRuleWord(word, nonterminalsSet, terminalsSet)) {
              this.handleInputMessageChange("Bad right part of the rule!");
              return;
            }
            rulesSet.push([nonterminal, word]);
          }

	/*
      switch (i % 3) {
        case 0:
          if (
            this.badLetterProblemNonterminals(rules[i]) ||
            !nonterminalsSet.includes(rules[i])
          ) {
            this.handleInputMessageChange("Bad left part of the rule!");
            return;
          }
          nonterminal = rules[i];
          break;
        case 1:
          if (!this.isEpsilon(rules[i])) {
            this.handleInputMessageChange("Bad format!");
            return;
          }
          break;
        case 2:
          for (let word of rules[i].split("|")) {
            if (this.badRuleWord(word, nonterminalsSet, terminalsSet)) {
              this.handleInputMessageChange("Bad right part of the rule!");
              return;
            }
            rulesSet.push([nonterminal, word]);
          }
          break;
        default:
          break;
      }
	*/
    }

    this.props.setGrammar({
      nonterminalsSet,
      terminalsSet,
      start,
      rulesSet
    });

    console.log(nonterminalsSet);
    console.log(terminalsSet);
    console.log(start);
    console.log(rulesSet);
    console.log(this.props);
  };

  render() {
    return (
      <form onSubmit={this.readInput}>
        <p>G = {"{"}</p>
        <div className="first">
          <label htmlFor="nonterminals">N = {"{ "}</label>
        </div>
        <div className="second">
          <input
            type="text"
            id="nonterminals"
            name="nonterminals"
            onChange={this.handleNonterminalsChange}
          />
          {" },"}
        </div>
        <div className="first">
          <label htmlFor="terminals">T = {"{ "}</label>
        </div>
        <div className="second">
          <input
            type="text"
            id="terminals"
            name="terminals"
            onChange={this.handleTerminalsChange}
          />
          {" },"}
        </div>
        <div className="first">
          <label htmlFor="select">S =</label>
        </div>
        <div className="second">
          <select
            id="select"
            value={this.state.start}
            onChange={this.handleStartChange}
          >
            <option key="">Select start...</option>
            {this.state.nonterminals.split(",").map((letter, key) => (
              <option key={key}>{letter}</option>
            ))}
          </select>
          {","}
        </div>
        <div className="first">
          <label htmlFor="rules">P = {"{ "}</label>
        </div>
        <div className="second">
          <textarea
            type="text"
            id="rules"
            name="rules"
            onChange={this.handleRulesChange}
          />
          {" }"}
        </div>
        <p>{"}"}</p>
        <input type="button" onClick={this.readInput} value="Enter" />
        <br />
        {this.props.grammar != null && (
          <ReducedFormButton
            grammar={this.props.grammar}
            setGrammar={this.props.setGrammar}
          />
        )}
        <br />
        {this.props.grammar != null && (
          <EpsilonFreeButton
            grammar={this.props.grammar}
            setGrammar={this.props.setGrammar}
          />
        )}
	<br />
        {this.props.grammar != null && (
          <ChomskyButton
            grammar={this.props.grammar}
            setGrammar={this.props.setGrammar}
          />
        )}

        {this.state.inputMessage !== "" && <h3>{this.state.inputMessage}</h3>}
      </form>
    );
  }
}

export default InputGrammarForms;
