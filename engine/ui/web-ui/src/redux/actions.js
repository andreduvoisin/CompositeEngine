export const AnimationMutationTypes = {

  TOGGLE_ANIMATION_REQUEST: 'TOGGLE_ANIMATION_REQUEST',
  TOGGLE_ANIMATION_SUCCESS: 'TOGGLE_ANIMATION_SUCCESS',
  TOGGLE_ANIMATION_FAILURE: 'TOGGLE_ANIMATION_FAILURE',

  PAUSE_STATE_UPDATE: 'PAUSE_STATE_UPDATE',

  ANIMATION_STATE_UPDATE: 'ANIMATION_STATE_UPDATE',

  FPS_COUNTER_STATE_UPDATE: 'FPS_COUNTER_STATE_UPDATE',

  SET_ANIMATION_TIME: 'SET_ANIMATION_TIME',

  SET_RENDER_MODE: 'SET_RENDER_MODE',
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

export const setAnimationTime = (time) => {
  return {
    type: AnimationMutationTypes.SET_ANIMATION_TIME,
    payload: {
      time: parseFloat(time)
    }
  };
};

export const setRenderSkeletonMode = (mode) => {
  return {
    type: AnimationMutationTypes.SET_RENDER_MODE,
    payload: {
      mode: parseInt(mode)
    }
  };
};

export const updateFpsCounterState = (fpsCounterState) => {
  return {
    type: AnimationMutationTypes.FPS_COUNTER_STATE_UPDATE,
    payload: fpsCounterState
  };
};
