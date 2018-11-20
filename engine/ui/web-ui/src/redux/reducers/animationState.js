import { AnimationMutationTypes } from "../actions";

const initialState = {
  isPlaying: true,
  currentTime: 0,
  duration: 0,
  fps: 0
};

export default (
  state = initialState,
  action
) => {

  switch (action.type) {

    case AnimationMutationTypes.PAUSE_STATE_UPDATE:
      return {
        ...state,
        isPlaying: action.payload.paused !== true
      };

    case AnimationMutationTypes.ANIMATION_STATE_UPDATE:
      return {
        ...state,
        currentTime: action.payload.currentTime,
        duration: action.payload.duration
      };

    case AnimationMutationTypes.FPS_COUNTER_STATE_UPDATE:
      return {
        ...state,
        fps: action.payload.fps
      };

    case AnimationMutationTypes.SET_RENDER_MODE:
      return {
        ...state,
        renderMode: action.payload.mode
      };

    default:
      return state;
  }
};
