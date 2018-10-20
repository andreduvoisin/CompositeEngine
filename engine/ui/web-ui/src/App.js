import React, { Component } from 'react';
import './App.css';
import Button from './Components/Button';
import Alert from './Components/Alert';

class App extends Component {
  render() {
    return (
      <div className='WebUi'>
        <Alert
          title='Attention'
          body='Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin efficitur ex non dui malesuada, vel mattis nunc accumsan.'
        >

        </Alert>
      </div>
    );
  }
}

export default App;
