import React, { Component } from "react";
import Legend from "../components/Legend";
import RelationPrinter from "./RelationPrinter";
import SyntaxAnimation from "./SyntaxAnimation";
import InputSyntaxAnalyzer from "./InputSyntaxAnalyzer";
import { grammarToString } from "../components/Utils/GrammarParser";
import { connect } from "react-redux";
import { epsilon } from "../components/Utils/Constants";
import { getWordSyntaxAnalysis } from "../../Actions/actions";

import "./SyntaxAnalyzer.css";

class SyntaxAnalyzer extends Component {
  constructor(props) {
    super(props);
    this.wordForAnalysis = React.createRef();
  }

  nonsenseWord = word => {
    for (let i = 0; i < word.length; i += 1) {
      if (
        this.props.grammarForAnalysis.grammar.terminalsSet.includes(word[i]) ===
        false
      )
        return true;
    }
    return false;
  };

  analyzeWord = () => {
    const word = this.wordForAnalysis.current.value;
    if (this.nonsenseWord(word) === true || word === epsilon) return;

    this.props.getWordSyntaxAnalysis(
      grammarToString(this.props.grammarForAnalysis.grammar),
      word
    );
  };

  render() {
    return (
      <div className="syntax-analyzer">
        <h1>Syntax Analyzer</h1>
        <div className="syntax-analyzer-sidebar">
          <InputSyntaxAnalyzer />
          {this.props.precedentialRelation !== null && <RelationPrinter />}
          {this.props.grammarForAnalysis !== null &&
            this.props.grammarForAnalysis.isPrecedential === true && (
              <div className="analysis-word">
                <h1>Word to analyze:</h1>
                <input
                  type="text"
                  className="word-for-analysis"
                  ref={this.wordForAnalysis}
                />
                <input
                  type="button"
                  onClick={this.analyzeWord}
                  value={"Analyze word"}
                />
              </div>
            )}
          {this.props.grammarForAnalysis !== null &&
            this.props.grammarForAnalysis.isPrecedential !== true && (
              <p>Grammar is not simple precedential.</p>
            )}
          {this.props.analysisOfWord !== null && <SyntaxAnimation />}
        </div>

        <Legend />
      </div>
    );
  }
}

export default connect(
  state => ({
    grammarForAnalysis: state.grammarForAnalysis,
    precedentialRelation: state.precedentialRelation,
    analysisOfWord: state.analysisOfWord,
    wordForAnalysis: state.wordForAnalysis
  }),
  { getWordSyntaxAnalysis }
)(SyntaxAnalyzer);
