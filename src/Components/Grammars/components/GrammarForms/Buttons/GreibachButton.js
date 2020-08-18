import React, { Component } from "react";
import { grammarToString } from "../../Utils/GrammarParser";
import { toGreibachNormalForm } from "../../../../Actions/actions";
import { connect } from "react-redux";

class GreibachButton extends Component {
  execute = () => {
    const grammarString = grammarToString(this.props.grammar);
    this.props.toGreibachNormalForm(grammarString, this.props.setGrammar);
  };

  render() {
    return (
      <input type="button" onClick={this.execute} value={"Greibach Normal Form"} />
    );
  }
}

export default connect(
  state => ({}),
  { toGreibachNormalForm }
)(GreibachButton);
