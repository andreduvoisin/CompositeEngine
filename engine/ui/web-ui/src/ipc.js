import eventemitter3 from "eventemitter3";
import cstruct from "c-struct";

export const togglePauseRequestSchema = new cstruct.Schema({
  id: cstruct.type.uint32,
  _: cstruct.type.uint8
});

export const togglePauseResponseSchema = new cstruct.Schema({
  isPaused: cstruct.type.uint8
});

const animationStateRequestSchema = new cstruct.Schema({
  id: cstruct.type.uint32,
  _: cstruct.type.uint8
});

const animationStateResponseSchema = new cstruct.Schema({
  currentTime: cstruct.type.uint32,
  duration: cstruct.type.uint32
});

cstruct.register('TogglePauseRequest', togglePauseRequestSchema);
cstruct.register('TogglePauseResponse', togglePauseResponseSchema);
cstruct.register('AnimationStateRequest', animationStateRequestSchema);
cstruct.register('AnimationStateResponse', animationStateResponseSchema);

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
  const message = cstruct.packSync('TogglePauseRequest', {
    id: 0
  });
  return sendMessage(message.toString()).then((data) => {
    const buf = Buffer.from(data);
    return cstruct.unpackSync('TogglePauseResponse', buf);
  });
};

export const subscribeToAnimationState = (handler) => {
  const message = cstruct.packSync('AnimationStateRequest', {
    id: 1
  });
  window.cefQuery({
    request: message.toString(),
    persistent: true,
    onSuccess: (success) => {
      const buf = Buffer.from(success);
      const ack = cstruct.unpackSync('AnimationStateResponse', buf);
      const data = JSON.stringify(ack);
      handler(data);
    },
    onFailure: (failure) => {
      console.log(failure);
    }
  });
};

subscribeToAnimationState((state) => {
  console.log("AnimationStateResponse: " + state);
});
