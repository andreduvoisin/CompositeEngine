import { connect } from 'react-redux';
import Taskbar from '../Components/Taskbar';
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
)(Taskbar);
