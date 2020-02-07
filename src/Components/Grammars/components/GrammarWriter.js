import React, { Component } from "react";
import { epsilon } from "./Utils/Constants";
import "./GrammarWriter.css";

class GrammarWriter extends Component {
  readInput = () => {
    console.log("grammarWriter: ", this.props);
  };

  writeSet = nonterminals => {
    let nonterminalsString = "";
    for (let i = 0; i < nonterminals.length; i += 1) {
      nonterminalsString += nonterminals[i];
      if (i !== nonterminals.length - 1) {
        nonterminalsString += ",";
      }
    }
    return nonterminalsString;
  };

  writeRules = rules => {
    let rulesString = "";
    let usedNonterminals = new Set();
    for (let i = 0; i < rules.length; i += 1) {
      if (usedNonterminals.has(rules[i][0])) {
        continue;
      }
      rulesString += rules[i][0] + "->";
      for (let j = 0; j < rules.length; j += 1) {
        if (rules[j][0] === rules[i][0]) {
          if (rules[j][1] === epsilon) {
            rulesString += "Epsilon|";
          } else {
            rulesString += rules[j][1] + "|";
          }
        }
      }
      rulesString = rulesString.slice(0, -1);
      rulesString += " , ";
      usedNonterminals.add(rules[i][0]);
    }
    rulesString =
      rulesString.length !== 0 ? rulesString.slice(0, -3) : rulesString;
    return rulesString;
  };

  render() {
    return (
      <div className="grammarwriter">
        {this.props.grammar != null && (
          <div>
            <h3>
              G = {"{"}
              <br /> N = {"{ "}
              {this.writeSet(this.props.grammar.nonterminalsSet)}
              {" },"}
              <br /> T = {"{ "}
              {this.writeSet(this.props.grammar.terminalsSet)}
              {" },"}
              <br /> S ={this.props.grammar.start}
              {","}
              <br /> P = {"{ "}
              {this.writeRules(this.props.grammar.rulesSet)}
              {" }"}
              {"}"}
            </h3>
          </div>
        )}
      </div>
    );
  }
}

export default GrammarWriter;
