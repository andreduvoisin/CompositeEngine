// import { createReducer } from 'reduxsauce';

import { Types } from "../actions";
import { createReducer } from "reduxsauce";

const INITIAL_STATE = {
  isPlaying: true,
  currentTime: 0,
  duration: 0,
  fps: 0
};

const HANDLERS = {

  [Types.PAUSE_STATE_UPDATE]: (state, action) => {
    return {
      ...state,
      isPlaying: action.payload.paused !== true
    };
  },

  [Types.ANIMATION_STATE_UPDATE]: (state, action) => {
    return {
      ...state,
      currentTime: action.payload.currentTime,
      duration: action.payload.duration
    };
  },

  [Types.FPS_COUNTER_STATE_UPDATE]: (state, action) => {
    return {
      ...state,
      fps: action.payload.fps
    };
  },

  [Types.SET_RENDER_MODE]: (state, action) => {
    return {
      ...state,
      renderMode: action.payload.mode
    };
  },

};

export default createReducer(INITIAL_STATE, HANDLERS);
