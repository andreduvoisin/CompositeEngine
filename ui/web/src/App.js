import 'font-awesome/css/font-awesome.min.css';
import 'react-toastify/dist/ReactToastify.css';
import './components/Toast/Toast.less';
import React, { Component } from 'react';
import { ToastContainer } from 'react-toastify';
import { ThemeProvider } from 'styled-components';
import DebugStyles from './components/Styles/DebugStyles';
import GlobalStyles from './components/Styles/GlobalStyles';
import Layout from './components/Layout/Layout';
import Toolbar from './components/Toolbar/Toolbar';
import Sidebar from './components/Sidebar/Sidebar';
import AnimationControls from './containers/AnimationControls';
import FpsCounter from "./containers/FpsCounter";
import theme from './theme';

const debugLayout = false;

class App extends Component {
  render() {
    return (
      <ThemeProvider theme={theme}>
        <>
          {(debugLayout) ? <DebugStyles /> : null}
          <GlobalStyles />
          <Toolbar>
            <AnimationControls />
          </Toolbar>
          <ToastContainer toastClassName="Toast" />
          <FpsCounter />
        </>
      </ThemeProvider>
    );
  }
}

export default App;
