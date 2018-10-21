import React, { Component } from 'react';
import './App.css';
import Button from './Components/Button';
import Alert from './Components/Alert';
import 'font-awesome/css/font-awesome.min.css';
import { resolve } from 'url';

const sendMessage = (payload) => {
  return new Promise((resolve, reject) => {
    window.cefQuery({
      request: payload,
      persistent: false,
      onSuccess: resolve,
      onFailure: reject
    });
  });
};

class App extends Component {
  render() {
    return (
      <div className='WebUi'>
        <Alert
          title='Attention'
          body='Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin efficitur ex non dui malesuada, vel mattis nunc accumsan.'
        >
          <Button onClick={() => {
            console.log("button press");
            sendMessage("my_request")
              .then((response) => {
                console.log(response);
              })
              .catch((e) => {
                console.error(e);
              });
          }}>Confirm</Button>
        </Alert>
      </div>
    );
  }
}

export default App;
