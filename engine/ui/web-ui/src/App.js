import 'font-awesome/css/font-awesome.min.css';
import React, { Component } from 'react';
import { createStore } from 'redux';
import { Provider as ReduxProvider } from 'react-redux'
import { ThemeProvider } from 'styled-components';
import BnetAlert from './Components/BnetAlert';
import BnetButton from './Components/BnetButton';
import DebugStyles from './Components/DebugStyles';
import GlobalStyles from './Components/GlobalStyles';
import Layout from './Components/Layout';
import Sidebar from './Components/Sidebar';
import Taskbar from './Components/Taskbar';
import reducer from './redux/reducers/index';
import theme from './theme';

const debugLayout = false;

// mount it on the Store
const store = createStore(
  reducer
);

class App extends Component {
  render() {
    return (
      <ThemeProvider theme={theme}>
        <ReduxProvider store={store}>
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
              <BnetAlert title='attention'>
                <BnetButton>OK</BnetButton>
              </BnetAlert>
            </Layout.FullHeightContainer>
          </React.Fragment>
        </ReduxProvider>
      </ThemeProvider>
    );
  }
}

export default App;
