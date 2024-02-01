import { useState } from 'react';
import logo from './logo.svg';
import './App.css';
import { Convert, Command,Function } from "common";

function App() {
  const responseBody: Command = { "function": Function.MoveBackward };
  const [response, setResponse] = useState<Command>(responseBody);


  async function handleJsonFromApi(json: any) {
    const command = Convert.toCommand(json);
    setResponse(command);
  }

  async function fetchApi() {
    try {
      const response = await fetch('/api/', {
        headers: { 'Accept': 'application/json', 'Content-Type': 'application/json' },
        method: "GET"
      });

      if (response.status === 200) {
        response.text().then(handleJsonFromApi);
      } else {
        setResponse({ "function": Function.MoveBackward })
      }
    } catch (e) {
      setResponse({ "function": Function.MoveBackward });
    }
  }

  return (
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <p>
          Edit <code>src/App.tsx</code> and save to reload.
        </p>
        <button onClick={() => fetchApi()}>Call API</button>
        <textarea readOnly={true} style={{ height: "200px" }} value={response.function}></textarea>
      </header>
    </div>
  );
}

export default App;
