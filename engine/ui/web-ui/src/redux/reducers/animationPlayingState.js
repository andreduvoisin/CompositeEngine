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

    case AnimationMutationTypes.PLAY:
      return AnimationPlayingStates.PLAYING;

    case AnimationMutationTypes.PAUSE:
      return AnimationPlayingStates.PAUSED;

    case AnimationMutationTypes.TOGGLE:
      return (state === AnimationPlayingStates.PLAYING)
        ? AnimationPlayingStates.PAUSED
        : AnimationPlayingStates.PLAYING;

    default:
      return state
  }
};
