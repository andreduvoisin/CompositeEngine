import { connect } from 'react-redux';
import { compose } from 'recompose';

const mapStateToProps = (state) => {
  return {
    fps: state.animationState.fps
  }
};

const mapDispatchToProps = (dispatch) => {
  return {};
};

export default (Component) => {
  return compose(
    connect(
      mapStateToProps,
      mapDispatchToProps
    )
  )(Component);
};
