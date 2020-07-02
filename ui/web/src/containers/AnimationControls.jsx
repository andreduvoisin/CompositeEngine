import React from 'react';
import { connect } from 'react-redux';
import AnimationControls from '../components/AnimationControls/AnimationControls';
import { Creators } from '../redux/actions';
const { toggleAnimationRequest, setAnimationTime, setRenderMode } = Creators;

const mapStateToProps = (state) => {
  return state;
};

const mapDispatchToProps = (dispatch) => {
  return {
    toggleAnimation: () => {
      dispatch(toggleAnimationRequest());
    },
    setAnimationTime: (event) => {
      dispatch(setAnimationTime(event.target.value));
    },
    setRenderMode: (value) => {
      dispatch(setRenderMode(value));
    }
  };
};

export default connect(
  mapStateToProps,
  mapDispatchToProps
)((props) => {
  const { animationState, setRenderMode, toggleAnimation, setAnimationTime } = props;
  const { isPlaying, animationStatus, currentTime, duration } = animationState;
  return (
    <AnimationControls
      {...{ isPlaying, animationStatus, currentTime, duration }}
      {...{ setRenderMode, toggleAnimation, setAnimationTime }}
    />
  );
});
