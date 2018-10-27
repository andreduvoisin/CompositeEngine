export const AnimationMutationTypes = {
  // toggle types
  TOGGLE_ANIMATION_REQUEST: 'TOGGLE_ANIMATION_REQUEST',
  TOGGLE_ANIMATION_SUCCESS: 'TOGGLE_ANIMATION_SUCCESS',
  TOGGLE_ANIMATION_FAILURE: 'TOGGLE_ANIMATION_FAILURE',
  ANIMATION_STATE_UPDATE: 'ANIMATION_STATE_UPDATE',
};

export const toggleAnimation = () => {
  return {
    type: AnimationMutationTypes.TOGGLE_ANIMATION_REQUEST
  }
};

export const toggleAnimationSuccess = (animationState) => {
  return {
    type: AnimationMutationTypes.TOGGLE_ANIMATION_SUCCESS,
    payload: animationState
  };
};

export const toggleAnimationFailure = () => {
  return {
    type: AnimationMutationTypes.TOGGLE_ANIMATION_FAILURE
  };
};

export const updateAnimationState = (animationState) => {
  return {
    type: AnimationMutationTypes.ANIMATION_STATE_UPDATE,
    payload: animationState
  };
};
