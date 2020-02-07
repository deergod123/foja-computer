import React, { Component } from "react";
import { connect } from "react-redux";

import "./RelationPrinter.css";

class RelationPrinter extends Component {
  render() {
    return (
      <div className="precedential-relation">
        {this.props.precedentialRelation !== null && (
          <div className="precedential-relation-sidebar">
            <h1>Precedential relation for Grammar:</h1>
            <p className="relation-paragraph">Less relation:</p>
            {this.props.precedentialRelation.lessRelation.map(
              (relation, key) => (
                <p key={key} className="relation-paragraph">
                  {"("} {relation.first}
                  {" , "} {relation.second}
                  {")"}
                </p>
              )
            )}
            <p className="relation-paragraph">Equals relation:</p>
            {this.props.precedentialRelation.equalsRelation.map(
              (relation, key) => (
                <p key={key} className="relation-paragraph">
                  {"("} {relation.first}
                  {" , "} {relation.second}
                  {")"}
                </p>
              )
            )}
            <p className="relation-paragraph">More relation:</p>
            {this.props.precedentialRelation.moreRelation.map(
              (relation, key) => (
                <p key={key} className="relation-paragraph">
                  {"("} {relation.first}
                  {" , "} {relation.second}
                  {")"}
                </p>
              )
            )}
          </div>
        )}
      </div>
    );
  }
}

export default connect(
  state => ({
    precedentialRelation: state.precedentialRelation
  }),
  {}
)(RelationPrinter);
