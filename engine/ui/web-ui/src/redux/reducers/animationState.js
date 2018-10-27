import { AnimationMutationTypes } from "../actions";

const initialState = {
  isPlaying: true,
  currentTime: 0,
  duration: 0
};

export default (
  state = initialState,
  action
) => {

  switch (action.type) {

    case AnimationMutationTypes.TOGGLE_ANIMATION_SUCCESS:
      return {
        ...state,
        isPlaying: action.payload.isPaused === 0
      };

    case AnimationMutationTypes.ANIMATION_STATE_UPDATE:
      return {
        ...state,
        currentTime: action.payload.currentTime,
        duration: action.payload.duration,
      };

    default:
      return state
  }
};
