import React, { Component } from "react";
import turingac from "./images/turingac.mp4";
import logo_uk from "./images/logo_uk.png";
import Navigation from "../Navigation/Navigation";

import "./MainPage.css";

class MainPage extends Component {
  render() {
    return (
      <div className="mainpage-container">
        <Navigation />
        <video loop muted autoPlay className="mainpage-container-video">
          <source src={turingac} type="video/mp4" />
          Your browser does not support the video tag.
        </video>
        <div className="mainpage-container-title">
          <img src={logo_uk} alt="logo_uk" />
        </div>
        <div className="mainpage-container-footer">
          <h2>
            Developed by Erik Kučák for: FMFI UK{" "}
            <img src={logo_uk} alt="logo_uk" />
          </h2>
        </div>
      </div>
    );
  }
}

export default MainPage;
