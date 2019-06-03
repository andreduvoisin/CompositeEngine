import { createActions } from 'reduxsauce';

const setAnimationTime = (time) => {
  return {
    type: 'SET_ANIMATION_TIME',
    payload: {
      time: parseFloat(time)
    }
  };
};

const setRenderMode = (mode) => {
  return {
    type: 'SET_RENDER_MODE',
    payload: {
      mode: parseInt(mode)
    }
  };
};

export const { Types, Creators } = createActions({
  toggleAnimationRequest: null,
  toggleAnimationSuccess: null,
  toggleAnimationFailure: null,
  pauseStateUpdate: ['payload'],
  animationStateUpdate: ['payload'],
  fpsCounterStateUpdate: ['payload'],
  setRenderMode,
  setAnimationTime
});
