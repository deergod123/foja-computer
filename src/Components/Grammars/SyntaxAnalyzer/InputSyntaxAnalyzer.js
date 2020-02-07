import React, { Component } from "react";
import { connect } from "react-redux";
import { isPrecedential } from "../../Actions/actions";
import { grammarToString } from "../components/Utils/GrammarParser";

import "./InputSyntaxAnalyzer.css";

class InputSyntaxAnalyzer extends Component {
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
    if (word.length !== 1) return true;
    return word.charCodeAt(0) < 97 || word.charCodeAt(0) > 122;
  };

  badLetterProblemNonterminals = word => {
    if (word.length !== 1) return true;
    return word.charCodeAt(0) < 65 || word.charCodeAt(0) > 90;
  };

  badRuleWord = (word, nonterminalsSet, terminalsSet) => {
    for (let letter of word) {
      if (!nonterminalsSet.includes(letter) && !terminalsSet.includes(letter))
        return true;
    }
    return false;
  };

  isEpsilon = word => {
    return word.length === 0;
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
    const rules = this.state.rules.split(/['->', '\n']/);

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
    for (let i = 0; i < rules.length; i += 1) {
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
    }

    const stringGrammar = grammarToString({
      nonterminalsSet,
      terminalsSet,
      start,
      rulesSet
    });
    console.log(stringGrammar);
    this.props.isPrecedential(stringGrammar);
  };

  render() {
    return (
      <form onSubmit={this.readInput}>
        <h2>Grammar to analyze</h2>
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
      </form>
    );
  }
}

export default connect(
  state => ({
    grammarForAnalysis: state.grammarForAnalysis,
    precedentialRelation: state.precedentialRelation
  }),
  { isPrecedential }
)(InputSyntaxAnalyzer);
