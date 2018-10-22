import { AnimationMutationTypes } from "../actions";

export const AnimationPlayingStates = {
  PLAYING: 'PLAYING',
  PAUSED: 'PAUSED'
};

export default (
  state = AnimationPlayingStates.PLAYING,
  action
) => {

  switch (action.type) {

    // case AnimationMutationTypes.TOGGLE_ANIMATION_REQUEST:
    //   console.log(AnimationMutationTypes.TOGGLE_ANIMATION_REQUEST);
    //   return state;

    case AnimationMutationTypes.TOGGLE_ANIMATION_SUCCESS:
      return (state === AnimationPlayingStates.PLAYING)
        ? AnimationPlayingStates.PAUSED
        : AnimationPlayingStates.PLAYING;

    // case AnimationMutationTypes.TOGGLE_ANIMATION_FAILURE:
    //   console.log(AnimationMutationTypes.TOGGLE_ANIMATION_FAILURE);

    default:
      return state
  }
};
