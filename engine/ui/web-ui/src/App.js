import React, { Component } from 'react';
import './App.css';
import 'font-awesome/css/font-awesome.min.css';
import { ThemeProvider } from 'styled-components';
import theme from './theme';

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
      <ThemeProvider theme={theme}>
        <div className='WebUi'>

        </div>
      </ThemeProvider>
    );
  }
}

export default App;
