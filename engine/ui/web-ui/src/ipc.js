import eventemitter3 from "eventemitter3";

const MessageId = Object.freeze({
  RESPONSE_SUCCESS: 0,
  RESPONSE_FAILURE: 1,

  REQUEST_TOGGLE_PAUSE: 2,
  SUBSCRIPTION_PAUSE_STATE: 3,
  STATUS_PAUSE_STATE: 4,

  REQUEST_SET_ANIMATION_TIME: 5,
  SUBSCRIPTION_ANIMATION_STATE: 6,
  STATUS_ANIMATION_STATE: 7
})

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
    id: MessageId.REQUEST_TOGGLE_PAUSE
  };
  return sendMessage(JSON.stringify(message)).then((data) => {
    return JSON.parse(data);
  });
};

export const sendSetAnimationTime = (time) => {
  const message = {
    id: MessageId.REQUEST_SET_ANIMATION_TIME,
    time: time
  };
  return sendMessage(JSON.stringify(message)).then((data) => {
    return JSON.parse(data);
  });
};

export const subscribeToPauseState = (handler) => {
  const message = {
    id: MessageId.SUBSCRIPTION_PAUSE_STATE
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
    id: MessageId.SUBSCRIPTION_ANIMATION_STATE
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

