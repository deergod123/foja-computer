import React, { Component } from "react";
import { NavLink, withRouter } from "react-router-dom";
import { NavItem, Nav } from "reactstrap";

import "./Navigation.css";

class Navigation extends Component {
  render() {
    return (
      <Nav className="navbar">
        <NavItem className="navitem">
          <NavLink to="/" className="nav-link">
            Home
          </NavLink>
        </NavItem>
        <NavItem className="navitem">
          <NavLink to="/grammarpage/grammar-forms" className="nav-link">
            Grammar forms
          </NavLink>
        </NavItem>
        <NavItem className="navitem">
          <NavLink to="/grammarpage/syntax-analyzer" className="nav-link">
            Syntax analyzer
          </NavLink>
        </NavItem>
        <NavItem className="navitem">
          <NavLink to="/automatapage" className="nav-link">
            Automata
          </NavLink>
        </NavItem>
        <NavItem className="navitem">
          <NavLink to="/turing_machinepage" className="nav-link">
            Turing machine
          </NavLink>
        </NavItem>
      </Nav>
    );
  }
}

export default withRouter(Navigation);
