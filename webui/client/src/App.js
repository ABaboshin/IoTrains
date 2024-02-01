// client/src/App.js

import React from "react";
import logo from "./logo.svg";
import "./App.css";
import { Convert } from "common";

function App() {
  const [data, setData] = React.useState(null);

  React.useEffect(() => {
    fetch("/api")
      .then((res) => res.text())
      .then((data) => setData(Convert.toCommand(data).function));
  }, []);

  return (
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <p>{!data ? "Loading..." : data}</p>
      </header>
    </div>
  );
}

export default App;
