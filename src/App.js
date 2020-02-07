import React, { Component } from "react";
import { Route } from "react-router-dom";
import { Switch } from "react-router";
import MainPage from "./Components/MainPage/MainPage";
import GrammarForms from "./Components/Grammars/components/GrammarForms/GrammarForms";
import SyntaxAnalyzer from "./Components/Grammars/SyntaxAnalyzer/SyntaxAnalyzer";
import Navigation from "./Components/Navigation/Navigation";

import "./App.css";
class App extends Component {
  render() {
    return (
      <div className="application-container">
        <Route path="/:something" component={Navigation} />
        <Route exact path="/" component={MainPage} />
        <Switch>
          <Route path="/grammarpage/grammar-forms" component={GrammarForms} />
          <Route
            path="/grammarpage/syntax-analyzer"
            component={SyntaxAnalyzer}
          />
        </Switch>
      </div>
    );
  }
}

export default App;
