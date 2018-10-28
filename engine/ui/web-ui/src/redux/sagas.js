import { all, fork, put, takeEvery } from 'redux-saga/effects';
import { sendToggleAnimationRequest } from "../ipc";
import { AnimationMutationTypes, toggleAnimationFailure, toggleAnimationSuccess } from "./actions";
import { toast } from 'react-toastify';

const {
  TOGGLE_ANIMATION_REQUEST,
  TOGGLE_ANIMATION_SUCCESS
} = AnimationMutationTypes;

function* requestToggleAnimationStateAsync() {
  try {
    yield sendToggleAnimationRequest();
    yield put(toggleAnimationSuccess());
  } catch (e) {
    yield put(toggleAnimationFailure());
  }
}

function* watchToggleAnimationSuccess() {
  yield takeEvery(TOGGLE_ANIMATION_SUCCESS, (action) => {
    // const { payload } = action;
    // if (payload.isPaused === 1) {
    //   toast.success("Animation Paused", {
    //     position: toast.POSITION.BOTTOM_RIGHT,
    //     autoClose: 2000
    //   });
    // } else if (payload.isPaused === 0) {
    //   toast.success("Animation Resumed", {
    //     position: toast.POSITION.BOTTOM_RIGHT,
    //     autoClose: 2000
    //   });
    // }
  });
};

function* watchToggleAnimation() {
  yield takeEvery(TOGGLE_ANIMATION_REQUEST, requestToggleAnimationStateAsync);
};

export default function* root() {
  yield all([
    fork(watchToggleAnimation),
    fork(watchToggleAnimationSuccess)
  ]);
};
