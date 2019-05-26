import { AnimationMutationTypes } from "../actions";

const initialState = {
  isPlaying: true,
  currentTime: 0,
  duration: 0,
  fps: 0,
  availableAnimations: []
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
      const availableAnimations = [
        {
          name: action.payload.availableAnimationName1,
          duration: action.payload.availableAnimationDuration1.toFixed(2)
        },
        {
          name: action.payload.availableAnimationName2,
          duration: action.payload.availableAnimationDuration2.toFixed(2)
        }
      ];

      return {
        ...state,
        currentTime: action.payload.currentTime,
        duration: action.payload.duration,
        availableAnimations
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

    case AnimationMutationTypes.AVAILABLE_ANIMATIONS_UPDATE:
      {
        const availableAnimation = {
          name: action.payload.name,
          duration: action.payload.duration
        };

        const availableAnimations = [...state.availableAnimations];
        availableAnimations.push(availableAnimation);

        return {
          ...state,
          availableAnimations
        };
      }

    default:
      return state;
  }
};
