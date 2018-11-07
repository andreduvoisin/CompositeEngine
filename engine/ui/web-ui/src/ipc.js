// import eventemitter3 from "eventemitter3";

const MessageTypes = Object.freeze({
	SET_ANIMATION_TIME: 0,
	REQUEST_ANIMATION_STATE: 1,
	ANIMATION_STATE: 2,
	TOGGLE_PAUSE: 3,
	REQUEST_PAUSE_STATE: 4,
	PAUSE_STATE: 5,
	TOGGLE_RENDER_MODE: 6
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

export const sendToggleRenderSkeletonRequest = () => {
  const message = {
    type: MessageTypes.TOGGLE_RENDER_MODE
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

