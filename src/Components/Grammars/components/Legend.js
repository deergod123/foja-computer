import React, { Component } from "react";
import Latex from "react-latex";

class Legend extends Component {
  render() {
    return (
      <h3 className="legend">
        Please, use format: <br />N = {"{"}
        A, B, C, ... {"}"}
        <br />T = {"{"}
        a, b, c, ... {"}"}
	<br />For more symbols use (Nx) or (Tx), where x is the symbol number (z=26), for example ..., (N27), ...
        <br />P = {"{"}S -> <Latex>$(N\cup T)^*$</Latex> | ...
        {"}"}
        <br /> {"{"}A -> <Latex>$(N\cup T)^*$</Latex> | ...
        {"}"}
      </h3>
    );
  }
}

export default Legend;
