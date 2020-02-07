import React, { Component } from "react";
import InputGrammar from "./InputGrammarForms";
import Legend from "../Legend";
import GrammarWriter from "../GrammarWriter";
import { connect } from "react-redux";
import { setGrammar1, setGrammar2 } from "../../../Actions/actions";
import EquivalentButton from "./Buttons/EquivalentButton";

import "./GrammarForms.css";

class GrammarForms extends Component {
  render() {
    return (
      <div className="grammar">
        <h1>Grammar Computer</h1>
        <div className="grammar-sidebar">
          <h2>Grammar 1:</h2>
          <InputGrammar
            grammar={this.props.grammar1}
            setGrammar={this.props.setGrammar1}
          />
          <GrammarWriter grammar={this.props.grammar1} />
        </div>
        <div className="grammar-sidebar2">
          <h2>Grammar 2:</h2>
          <InputGrammar
            grammar={this.props.grammar2}
            setGrammar={this.props.setGrammar2}
          />
          <GrammarWriter grammar={this.props.grammar2} />
        </div>
        <Legend />
        {this.props.grammar1 != null && this.props.grammar2 != null && (
          <EquivalentButton
            grammar1={this.props.grammar1}
            grammar2={this.props.grammar2}
          />
        )}
        {this.props.equivalencyAnswer !== null &&
          this.props.equivalencyAnswer.equivalency === true && (
            <p className="eqP">
              Grammars are equivalent for all words with size{" "}
              {this.props.equivalencyAnswer.answer}
            </p>
          )}
        {this.props.equivalencyAnswer !== null &&
          this.props.equivalencyAnswer.equivalency === false && (
            <p className="eqP">
              Grammars are not equivalent, because of word{" "}
              {this.props.equivalencyAnswer.answer}
            </p>
          )}
      </div>
    );
  }
}

export default connect(
  state => ({
    grammar1: state.grammar1,
    grammar2: state.grammar2,
    equivalencyAnswer: state.equivalencyAnswer
  }),
  { setGrammar1, setGrammar2 }
)(GrammarForms);
