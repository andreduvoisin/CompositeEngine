import 'font-awesome/css/font-awesome.min.css';
import React, { Component } from 'react';
import { ThemeProvider } from 'styled-components';
import DebugStyles from './Components/DebugStyles';
import GlobalStyles from './Components/GlobalStyles';
import Layout from './Components/Layout';
import Taskbar from './Components/Taskbar';
import theme from './theme';

const debugLayout = false;

class App extends Component {
  render() {
    return (
      <ThemeProvider theme={theme}>
        <React.Fragment>
          {(debugLayout) ? <DebugStyles /> : null}
          <GlobalStyles />
          <Taskbar />
          <Layout.FullHeightContainer>
            {/* <Layout.Sidebar>
                <Sidebar>
                  <span>Sidebar</span>
                </Sidebar>
              </Layout.Sidebar> */}
            <Layout.Center />
            {/* <Layout.Sidebar>
                <Sidebar>
                  <span>Sidebar</span>
                </Sidebar>
              </Layout.Sidebar> */}
          </Layout.FullHeightContainer>
        </React.Fragment>
      </ThemeProvider>
    );
  }
}

export default App;
