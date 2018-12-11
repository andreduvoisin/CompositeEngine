import { all, fork, put, takeEvery } from 'redux-saga/effects';
import {
  AnimationMutationTypes,
  toggleAnimationFailure,
  toggleAnimationSuccess
} from "./actions";
import {
  sendToggleAnimationRequest,
  sendSetAnimationTime,
  sendSetRenderModeRequest
} from "../ipc";

const {
  TOGGLE_ANIMATION_REQUEST,
  SET_RENDER_MODE,
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
  yield sendSetAnimationTime(action.payload.time);
}

function* requestToggleRenderSkeleton(action) {
  yield sendSetRenderModeRequest(action.payload.mode);
}

function* watchToggleAnimation() {
  yield takeEvery(TOGGLE_ANIMATION_REQUEST, requestToggleAnimationStateAsync);
};

function* watchSetAnimationTime() {
  yield takeEvery(SET_ANIMATION_TIME, requestSetAnimationTime);
};

function* watchToggleRenderSkeleton() {
  yield takeEvery(SET_RENDER_MODE, requestToggleRenderSkeleton);
};

export default function* root() {
  yield all([
    fork(watchToggleAnimation),
    fork(watchSetAnimationTime),
    fork(watchToggleRenderSkeleton)
  ]);
};
