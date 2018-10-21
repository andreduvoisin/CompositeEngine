import 'font-awesome/css/font-awesome.min.css';
import React, { Component } from 'react';
import { ThemeProvider } from 'styled-components';
import theme from './theme';
import BnetAlert from './Components/BnetAlert';
import BnetButton from './Components/BnetButton';
import Sidebar from './Components/Sidebar';
import TaskBar from './Components/TaskBar';
import Layout from './Components/Layout';
import GlobalStyles from './Components/GlobalStyles';
import DebugStyles from './Components/DebugStyles';

const debugLayout = false;

class App extends Component {
  render() {
    return (
      <ThemeProvider theme={theme}>
        <React.Fragment>
          {(debugLayout) ? <DebugStyles /> : null}
          <GlobalStyles />
          <TaskBar>TaskBar</TaskBar>
          <Layout.Container>
            <Layout.Sidebar>
              <Sidebar>
                <span>Sidebar</span>
              </Sidebar>
            </Layout.Sidebar>
            <Layout.Center />
            <Layout.Sidebar>
              <Sidebar>
                <span>Sidebar</span>
              </Sidebar>
            </Layout.Sidebar>
            <BnetAlert title='attention'>
              <BnetButton>OK</BnetButton>
            </BnetAlert>
          </Layout.Container>
        </React.Fragment>
      </ThemeProvider>
    );
  }
}

export default App;
