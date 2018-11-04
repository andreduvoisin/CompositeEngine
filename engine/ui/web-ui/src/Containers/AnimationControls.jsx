import React from 'react';
import { connect } from 'react-redux';
import AnimationControls from '../Components/AnimationControls';
import { toggleAnimation, setAnimationTime } from "../redux/actions";

const mapStateToProps = (state) => {
  return state;
};

const mapDispatchToProps = (dispatch) => {
  return {
    toggleAnimation: () => {
      dispatch(toggleAnimation());
    },
    setAnimationTime: (event) => {
      dispatch(setAnimationTime(event.target.value));
    }
  };
};

export default connect(
  mapStateToProps,
  mapDispatchToProps
)((props) => {
  return (
    <AnimationControls
      isPlaying={props.animationState.isPlaying}
      currentTime={props.animationState.currentTime}
      duration={props.animationState.duration}
      {...props}
    />
  );
});
