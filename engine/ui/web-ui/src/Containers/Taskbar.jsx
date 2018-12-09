import { connect } from 'react-redux';
import Toolbar from '../Components/Toolbar';
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
)(Toolbar);
