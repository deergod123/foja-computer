import React, { Component } from "react";
import { grammarToString } from "../../Utils/GrammarParser";
import { toReducedNormalForm } from "../../../../Actions/actions";
import { connect } from "react-redux";

class ReducedFormButton extends Component {
  execute = () => {
    const grammarString = grammarToString(this.props.grammar);
    this.props.toReducedNormalForm(grammarString, this.props.setGrammar);
  };

  render() {
    return (
      <input
        type="button"
        onClick={this.execute}
        value={"Reduced Normal Form"}
      />
    );
  }
}

export default connect(
  state => ({}),
  { toReducedNormalForm }
)(ReducedFormButton);
