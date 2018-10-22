export const AnimationMutationTypes = {
  PLAY: 'PLAY_ANIMATION',
  PAUSE: 'PAUSE_ANIMATION',
  TOGGLE: 'TOGGLE_ANIMATION',
  RESET: 'RESET_ANIMATION',
};

export const pauseAnimation = () => {
  return {
    type: AnimationMutationTypes.PAUSE
  };
};

export const playAnimation = () => {
  return {
    type: AnimationMutationTypes.PLAY
  };
};

export const toggleAnimation = () => {
  return {
    type: AnimationMutationTypes.TOGGLE
  };
};

export const resetAnimation = () => {
  return {
    type: AnimationMutationTypes.RESET
  };
};
