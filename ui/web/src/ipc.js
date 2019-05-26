// import eventemitter3 from "eventemitter3";

const MessageTypes = Object.freeze({
  SET_ANIMATION_TIME: 0,
  REQUEST_ANIMATION_STATE: 1,
  ANIMATION_STATE: 2,
  TOGGLE_PAUSE: 3,
  REQUEST_PAUSE_STATE: 4,
  PAUSE_STATE: 5,
  SET_RENDER_MODE: 6,
  REQUEST_FPS_STATE: 7,
  FPS_STATE: 8,
  TOGGLE_BIND_POSE: 9,
	AVAILABLE_ANIMATION: 10,
  REQUEST_AVAILABLE_ANIMATION: 11,
  SET_ANIMATION: 12,
});

export const sendMessage = (action) => {
  return new Promise((resolve, reject) => {
    window.cefQuery({
      request: action,
      persistent: false,
      onSuccess: resolve,
      onFailure: reject
    });
  });
};

export const sendToggleAnimationRequest = () => {
  const message = {
    type: MessageTypes.TOGGLE_PAUSE
  };
  return sendMessage(JSON.stringify(message)).then((data) => {
    return JSON.parse(data);
  });
};

export const sendSetRenderModeRequest = (mode) => {
  const message = {
    type: MessageTypes.SET_RENDER_MODE,
    mode: mode
  };
  return sendMessage(JSON.stringify(message)).then((data) => {
    return JSON.parse(data);
  });
};

export const sendSetAnimationTime = (time) => {
  const message = {
    type: MessageTypes.SET_ANIMATION_TIME,
    time: time
  };
  return sendMessage(JSON.stringify(message)).then((data) => {
    return JSON.parse(data);
  });
};

export const sendSetAnimationRequest = (animationName) => {
  const message = {
    type: MessageTypes.SET_ANIMATION,
    name: animationName
  };
  return sendMessage(JSON.stringify(message)).then((data) => {
    return JSON.parse(data);
  });
};

export const subscribeToPauseState = (handler) => {
  const message = {
    type: MessageTypes.REQUEST_PAUSE_STATE
  };
  window.cefQuery({
    request: JSON.stringify(message),
    persistent: true,
    onSuccess: (data) => {
      handler(JSON.parse(data));
    },
    onFailure: (data) => {
      console.log(data);
    }
  });
};

export const subscribeToAnimationState = (handler) => {
  const message = {
    type: MessageTypes.REQUEST_ANIMATION_STATE
  };
  window.cefQuery({
    request: JSON.stringify(message),
    persistent: true,
    onSuccess: (data) => {
      handler(JSON.parse(data));
    },
    onFailure: (data) => {
      console.log(data);
    }
  });
};

export const subscribeToFpsCounterState = (handler) => {
  const message = {
    type: MessageTypes.REQUEST_FPS_STATE
  };
  window.cefQuery({
    request: JSON.stringify(message),
    persistent: true,
    onSuccess: (data) => {
      handler(JSON.parse(data));
    },
    onFailure: (data) => {
      console.log(data);
    }
  });
};

export const subscribeToAvailableAnimation = (handler) => {
  const message = {
    type: MessageTypes.REQUEST_AVAILABLE_ANIMATION
  };
  window.cefQuery({
    request: JSON.stringify(message),
    persistent: true,
    onSuccess: (data) => {
      handler(JSON.parse(data));
    },
    onFailure: (data) => {
      console.log(data);
    }
  });
};
