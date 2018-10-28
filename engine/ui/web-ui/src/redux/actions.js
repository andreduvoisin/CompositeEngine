export const AnimationMutationTypes = {
  // toggle types
  TOGGLE_ANIMATION_REQUEST: 'TOGGLE_ANIMATION_REQUEST',
  TOGGLE_ANIMATION_SUCCESS: 'TOGGLE_ANIMATION_SUCCESS',
  TOGGLE_ANIMATION_FAILURE: 'TOGGLE_ANIMATION_FAILURE',
  PAUSE_STATE_UPDATE: 'PAUSE_STATE_UPDATE',
  ANIMATION_STATE_UPDATE: 'ANIMATION_STATE_UPDATE',
}

export const toggleAnimation = () => {
  return {
    type: AnimationMutationTypes.TOGGLE_ANIMATION_REQUEST
  }
};

export const toggleAnimationSuccess = () => {
  return {
    type: AnimationMutationTypes.TOGGLE_ANIMATION_SUCCESS
  };
};

export const toggleAnimationFailure = () => {
  return {
    type: AnimationMutationTypes.TOGGLE_ANIMATION_FAILURE
  };
};

export const updatePauseState = (pauseState) => {
  return {
    type: AnimationMutationTypes.PAUSE_STATE_UPDATE,
    payload: pauseState
  };
};

export const updateAnimationState = (animationState) => {
  return {
    type: AnimationMutationTypes.ANIMATION_STATE_UPDATE,
    payload: animationState
  };
};
