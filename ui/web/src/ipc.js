export const MessageTypes = Object.freeze({
  SET_ANIMATION_TIME: 0,
  REQUEST_ANIMATION_STATE: 1,
  ANIMATION_STATE: 2,
  TOGGLE_PAUSE: 3,
  REQUEST_PAUSE_STATE: 4,
  PAUSE_STATE: 5,
  SET_RENDER_MODE: 6,
  REQUEST_FPS_STATE: 7,
  FPS_STATE: 8
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

export const subscribeToMessage = (messageType, handler) => {
  const message = {
    type: messageType
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
