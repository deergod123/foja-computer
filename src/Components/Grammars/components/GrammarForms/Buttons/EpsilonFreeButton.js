import React, { Component } from "react";
import { grammarToString } from "../../Utils/GrammarParser";
import { toEpsilonFreeForm } from "../../../../Actions/actions";
import { connect } from "react-redux";

class EpsilonFreeButton extends Component {
  execute = () => {
    const grammarString = grammarToString(this.props.grammar);
    this.props.toEpsilonFreeForm(grammarString, this.props.setGrammar);
  };

  render() {
    return (
      <input type="button" onClick={this.execute} value={"Epsilon Free Form"} />
    );
  }
}

export default connect(
  state => ({}),
  { toEpsilonFreeForm }
)(EpsilonFreeButton);
