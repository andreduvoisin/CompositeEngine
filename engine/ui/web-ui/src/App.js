import React, { Component } from 'react';
import 'font-awesome/css/font-awesome.min.css';
import theme from './theme';
import TaskBar from './Components/TaskBar';
import BnetAlert from './Components/BnetAlert';
import BnetButton from './Components/BnetButton';

import { createGlobalStyle, ThemeProvider } from 'styled-components'

const GlobalStyle = createGlobalStyle`
  body {
    font-family: ${props => props.theme.fonts.family};
    background-color: ${props => props.theme.colors.transparentBlack};
  }
`;

class App extends Component {
  render() {
    return (
      <ThemeProvider theme={theme}>
        <div className='WebUi'>
          <GlobalStyle />
          <TaskBar />
          <BnetAlert title='attention'>
            <BnetButton>OK</BnetButton>
          </BnetAlert>
        </div>
      </ThemeProvider>
    );
  }
}

export default App;
