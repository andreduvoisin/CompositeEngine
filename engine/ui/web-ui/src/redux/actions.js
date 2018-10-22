export const AnimationMutationTypes = {
  // toggle types
  TOGGLE_ANIMATION_REQUEST: 'TOGGLE_ANIMATION_REQUEST',
  TOGGLE_ANIMATION_SUCCESS: 'TOGGLE_ANIMATION_SUCCESS',
  TOGGLE_ANIMATION_FAILURE: 'TOGGLE_ANIMATION_FAILURE',
  // reset types
  RESET_ANIMATION_REQUEST: 'RESET_ANIMATION_REQUEST',
  RESET_ANIMATION_SUCCESS: 'RESET_ANIMATION_SUCCESS',
  RESET_ANIMATION_FAILURE: 'RESET_ANIMATION_FAILURE',
};

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

export const resetAnimation = () => {
  return {
    type: AnimationMutationTypes.RESET_ANIMATION_REQUEST
  };
};

export const resetAnimationSuccess = () => {
  return {
    type: AnimationMutationTypes.RESET_ANIMATION_SUCCESS
  };
};

export const resetAnimationFailure = () => {
  return {
    type: AnimationMutationTypes.RESET_ANIMATION_FAILURE
  };
};
