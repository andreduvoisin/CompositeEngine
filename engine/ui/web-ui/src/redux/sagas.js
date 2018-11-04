import { all, fork, put, takeEvery } from 'redux-saga/effects';
import { sendToggleAnimationRequest, sendSetAnimationTime } from "../ipc";
import { AnimationMutationTypes, toggleAnimationFailure, toggleAnimationSuccess } from "./actions";

const {
  TOGGLE_ANIMATION_REQUEST,
  SET_ANIMATION_TIME
} = AnimationMutationTypes;

function* requestToggleAnimationStateAsync() {
  try {
    yield sendToggleAnimationRequest();
    yield put(toggleAnimationSuccess());
  } catch (e) {
    yield put(toggleAnimationFailure());
  }
}

function* requestSetAnimationTime(action) {
  yield sendSetAnimationTime(action.payload);
}

function* watchToggleAnimation() {
  yield takeEvery(TOGGLE_ANIMATION_REQUEST, requestToggleAnimationStateAsync);
};

function* watchSetAnimationTime() {
  yield takeEvery(SET_ANIMATION_TIME, requestSetAnimationTime);
};

export default function* root() {
  yield all([
    fork(watchToggleAnimation),
    fork(watchSetAnimationTime),
  ]);
};
