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

const cstruct = require('c-struct');

const togglePauseRequestSchema = new cstruct.Schema({
  id: cstruct.type.uint32,
  _: cstruct.type.uint8
});

const togglePauseResponseSchema = new cstruct.Schema({
  isPaused: cstruct.type.uint8
});

cstruct.register('TogglePauseRequest', togglePauseRequestSchema);
cstruct.register('TogglePauseResponse', togglePauseResponseSchema);

const message = cstruct.packSync('TogglePauseRequest', {
  id: 0
});

sendMessage(message.toString())
  .then(success => {
    const buf = Buffer.from(success);
    const ack = cstruct.unpackSync('TogglePauseResponse', buf);;;;
    console.log("TogglePauseResponse: " + JSON.stringify(ack));
  })
  .catch(failure => {
    console.log(failure);
  });
