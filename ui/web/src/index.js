import { default as React } from 'react';
import ReactDOM from 'react-dom';
import { Provider as ReduxProvider } from 'react-redux';
import { applyMiddleware, createStore } from 'redux';
import { composeWithDevTools } from 'remote-redux-devtools';
import createSagaMiddleware from 'redux-saga';
import App from './App';
import './index.css';
import reducer from './redux/reducers/index';
import { Creators } from './redux/actions';
import rootSaga from './redux/sagas';
import { MessageTypes, subscribeToMessage } from './ipc';
const {
  pauseStateUpdate,
  animationStateUpdate,
  fpsCounterStateUpdate
} = Creators;

// create the saga middleware
const sagaMiddleware = createSagaMiddleware();

// create the redux store
const enhancers = composeWithDevTools(applyMiddleware(sagaMiddleware));

const store = createStore(reducer, enhancers);

sagaMiddleware.run(rootSaga);

subscribeToMessage(MessageTypes.REQUEST_PAUSE_STATE, (state) => {
  store.dispatch(pauseStateUpdate(state));
});

subscribeToMessage(MessageTypes.REQUEST_ANIMATION_STATE, (state) => {
  store.dispatch(animationStateUpdate(state));
});

subscribeToMessage(MessageTypes.REQUEST_FPS_STATE, (state) => {
  store.dispatch(fpsCounterStateUpdate(state));
});

ReactDOM.render(
  <ReduxProvider store={store}>
    <App />
  </ReduxProvider>,
  document.getElementById('root')
);
