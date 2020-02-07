import React, { Component } from "react";
import { grammarToString } from "../../Utils/GrammarParser";
import { isEquivalent } from "../../../../Actions/actions";
import { connect } from "react-redux";

class EquivalentButton extends Component {
  execute = () => {
    const grammarString1 = grammarToString(this.props.grammar1);
    const grammarString2 = grammarToString(this.props.grammar2);
    this.props.isEquivalent(grammarString1, grammarString2);
  };

  render() {
    return (
      <input
        type="button"
        id="eq"
        onClick={this.execute}
        value={"Equivalence"}
      />
    );
  }
}

export default connect(
  state => ({}),
  { isEquivalent }
)(EquivalentButton);
