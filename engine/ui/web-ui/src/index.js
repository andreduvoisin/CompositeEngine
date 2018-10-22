import { default as React } from 'react';
import ReactDOM from 'react-dom';
import { applyMiddleware, createStore } from 'redux';
import { Provider as ReduxProvider } from 'react-redux';
import createSagaMiddleware from 'redux-saga';
import App from './App';
import './index.css';
import reducer from './redux/reducers/index';
import sagas from './redux/sagas';
import * as serviceWorker from './serviceWorker';

// create the saga middleware
const sagaMiddleware = createSagaMiddleware()

// create the redux store
const store = createStore(
  reducer,
  applyMiddleware(sagaMiddleware)
);

sagaMiddleware.run(sagas);

ReactDOM.render(
  <ReduxProvider store={store}>
    <App />
  </ReduxProvider>,
  document.getElementById('root'));

// If you want your app to work offline and load faster, you can change
// unregister() to register() below. Note this comes with some pitfalls.
// Learn more about service workers: http://bit.ly/CRA-PWA
serviceWorker.unregister();
