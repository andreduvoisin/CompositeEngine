import React from 'react';
import { connect } from 'react-redux';
import AnimationControls from '../components/AnimationControls/AnimationControls';
import { Creators } from '../redux/actions';

const { toggleAnimationRequest, setAnimationTime, setRenderMode } = Creators;

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
  null,
  mapDispatchToProps
)((props) => {
  return (
    <AnimationControls {...props } />
  );
});
