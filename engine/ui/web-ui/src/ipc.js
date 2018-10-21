const sendMessage = (payload) => {
  return new Promise((resolve, reject) => {
    window.cefQuery({
      request: payload,
      persistent: false,
      onSuccess: resolve,
      onFailure: reject
    });
  });
};

export default sendMessage;
