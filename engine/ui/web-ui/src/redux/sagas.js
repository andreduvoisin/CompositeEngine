import { all, fork, put, takeEvery } from 'redux-saga/effects';
import { sendMessage } from "../ipc";
import { AnimationMutationTypes, resetAnimationFailure, resetAnimationSuccess, toggleAnimationFailure, toggleAnimationSuccess } from "./actions";

const {
  TOGGLE_ANIMATION_REQUEST,
  RESET_ANIMATION_REQUEST
} = AnimationMutationTypes;

function* requestToggleAnimationStateAsync() {
  try {
    yield sendMessage(TOGGLE_ANIMATION_REQUEST);
    yield put(toggleAnimationSuccess());
  } catch (e) {
    yield put(toggleAnimationFailure());
  }
}

function* requesResetAnimationStateAsync() {
  try {
    yield sendMessage(RESET_ANIMATION_REQUEST);
    yield put(resetAnimationSuccess());
  } catch (e) {
    yield put(resetAnimationFailure());
  }
}

function* watchToggleAnimation() {
  yield takeEvery(TOGGLE_ANIMATION_REQUEST, requestToggleAnimationStateAsync);
};

function* watchResetAnimation() {
  yield takeEvery(RESET_ANIMATION_REQUEST, requesResetAnimationStateAsync);
};

export default function* root() {
  yield all([
    fork(watchToggleAnimation),
    fork(watchResetAnimation)
  ]);
};
