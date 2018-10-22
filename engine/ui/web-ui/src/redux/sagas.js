import { takeEvery } from 'redux-saga/effects';
import { sendMessage } from "../ipc";
import { AnimationMutationTypes } from "./actions";

const {
  PAUSE,
  PLAY,
  RESET,
} = AnimationMutationTypes;

function* sagas() {

  yield takeEvery(PAUSE, () => {
    return sendMessage(PAUSE);
  });

  yield takeEvery(PLAY, () => {
    return sendMessage(PLAY);
  });

  yield takeEvery(RESET, () => {
    return sendMessage(RESET);
  });
}

export default sagas;
