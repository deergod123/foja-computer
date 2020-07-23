import React, { Component } from "react";
import { grammarToString } from "../../Utils/GrammarParser";
import { toEpsilonFreeForm } from "../../../../Actions/actions";
import { connect } from "react-redux";

class ChomskyButton extends Component {
  execute = () => {
    const grammarString = grammarToString(this.props.grammar);
    this.props.toChomskyNormalForm(grammarString, this.props.setGrammar);
  };

  render() {
    return (
      <input type="button" onClick={this.execute} value={"Chomsky Normal Form"} />
    );
  }
}

export default connect(
  state => ({}),
  { toEpsilonFreeForm }
)(EpsilonFreeButton);
