import React from 'react';
import { connect } from 'react-redux';
import AnimationControls from '../Components/AnimationControls';
import { toggleAnimation } from "../redux/actions";

const mapStateToProps = (state) => {
  return state;
};

const mapDispatchToProps = (dispatch) => {
  return {
    toggleAnimation: () => {
      dispatch(toggleAnimation())
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
