import React, { Component } from "react";
import { connect } from "react-redux";

import "./SyntaxAnimation.css";

class SyntaxAnimation extends Component {
  render() {
    return (
      <div className="syntax-animation">
        <h1>Syntax analysis for word "{this.props.wordForAnalysis}":</h1>
        {this.props.analysisOfWord.map((command, key) => (
          <p key={key} className="command-paragraph">
            {command}
          </p>
        ))}
      </div>
    );
  }
}

export default connect(state => ({
  analysisOfWord: state.analysisOfWord,
  wordForAnalysis: state.wordForAnalysis
}))(SyntaxAnimation);
