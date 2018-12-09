import 'font-awesome/css/font-awesome.min.css';
import 'react-toastify/dist/ReactToastify.css';
import './Components/Toast.less';
import React, { Component } from 'react';
import { ToastContainer } from 'react-toastify';
import { ThemeProvider } from 'styled-components';
import DebugStyles from './Components/DebugStyles';
import GlobalStyles from './Components/GlobalStyles';
import Layout from './Components/Layout';
import Toolbar from './Components/Toolbar';
import Sidebar from './Components/Sidebar';
import AnimationControls from './Containers/AnimationControls';
import theme from './theme';

const debugLayout = false;

class App extends Component {
  render() {
    return (
      <ThemeProvider theme={theme}>
        <React.Fragment>
          {(debugLayout) ? <DebugStyles /> : null}
          <GlobalStyles />
          <Toolbar></Toolbar>
          <Toolbar>
            <AnimationControls />
          </Toolbar>
          <Layout.FullHeightContainer>
            <Layout.Sidebar width={90}>
              <Sidebar></Sidebar>
            </Layout.Sidebar>
            <Layout.Center />
            {/* <Layout.Sidebar>
                <Sidebar>
                  <span>Sidebar</span>
                </Sidebar>
              </Layout.Sidebar> */}
          </Layout.FullHeightContainer>
          <ToastContainer toastClassName="Toast" />
        </React.Fragment>
      </ThemeProvider>
    );
  }
}

export default App;
