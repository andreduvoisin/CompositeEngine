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

const simpleMessageSchema = new cstruct.Schema({
  id: cstruct.type.uint32,
  dickSize: cstruct.type.uint8,
  dickSize2: cstruct.type.uint8,
  dickSize3: cstruct.type.uint8,
  dickSize4: cstruct.type.uint8
});

const simpleMessageAckSchema = new cstruct.Schema({
  dickSize: cstruct.type.uint8,
  dickSize2: cstruct.type.uint8,
  dickSize3: cstruct.type.uint8,
  dickSize4: cstruct.type.uint8
});

cstruct.register('SimpleMessage', simpleMessageSchema);
cstruct.register('SimpleMessageAck', simpleMessageAckSchema);

const message = cstruct.packSync('SimpleMessage', {
  id: 3,
  dickSize: 69,
  dickSize2: 69,
  dickSize3: 69,
  dickSize4: 69
});

sendMessage(message.toString())
  .then(success => {
    const buf = Buffer.from(success);
    const ack = cstruct.unpackSync('SimpleMessageAck', buf);
    console.log("ack: " + JSON.stringify(ack));
  })
  .catch(failure => {
    console.log(failure);
  });
