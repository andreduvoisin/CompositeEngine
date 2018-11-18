import { all, fork, put, takeEvery } from 'redux-saga/effects';
import {
  AnimationMutationTypes,
  toggleAnimationFailure,
  toggleAnimationSuccess
} from "./actions";
import {
  sendToggleAnimationRequest,
  sendSetAnimationTime,
  sendToggleRenderSkeletonRequest
} from "../ipc";

const {
  TOGGLE_ANIMATION_REQUEST,
  TOGGLE_RENDER_SKELETON,
  SET_ANIMATION_TIME,
  SET_FPS_COUNTER
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

function* requestToggleRenderSkeleton(action) {
  yield sendToggleRenderSkeletonRequest();
}

function* watchToggleAnimation() {
  yield takeEvery(TOGGLE_ANIMATION_REQUEST, requestToggleAnimationStateAsync);
};

function* watchSetAnimationTime() {
  yield takeEvery(SET_ANIMATION_TIME, requestSetAnimationTime);
};

function* watchToggleRenderSkeleton() {
  yield takeEvery(TOGGLE_RENDER_SKELETON, requestToggleRenderSkeleton);
};

function* watchSetFpsCounter() {
  // yield takeEvery(SET_FPS_COUNTER, requestFpsCounterState);
};

export default function* root() {
  yield all([
    fork(watchToggleAnimation),
    fork(watchSetAnimationTime),
    // fork(watchSetFpsCounter),
    fork(watchToggleRenderSkeleton)
  ]);
};
