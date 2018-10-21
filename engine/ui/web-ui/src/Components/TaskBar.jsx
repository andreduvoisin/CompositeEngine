import React from 'react';
import { connect } from 'react-redux'
import styled from 'styled-components';
import { toggleAnimation } from "../redux/actions";
import { AnimationPlayingStates } from "../redux/reducers/animationPlayingState";

import Layout from './Layout';

const Container = styled.div`
    background: ${props => props.theme.shared.background};
    border: 1px solid rgba(255, 255, 255, .15);
    box-shadow: 1px 2px rgba(0, 0, 0, .5);
    font-family: 'Encode Sans', sans-serif;
    color: white;
`;

const InnerContainer = styled.div`
    padding: 16px;
`;

const PlayButton = styled.i`
  cursor: pointer;
  margin-right: auto;
  margin-left: auto;
  display: block;
  width: 10px;
`;

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

const Taskbar = (props) => {
  return (
    <Container>
      <InnerContainer>
        <Layout.Container>
          <Layout.Sidebar></Layout.Sidebar>
          <Layout.Center>
            {
              (props.animationPlayingState === AnimationPlayingStates.PLAYING)
                ? <PlayButton className="fa fa-play" onClick={props.toggleAnimation}></PlayButton>
                : <PlayButton className="fa fa-pause" onClick={props.toggleAnimation}></PlayButton>
            }
          </Layout.Center>
          <Layout.Sidebar></Layout.Sidebar>
        </Layout.Container>
      </InnerContainer>
    </Container>
  );
};

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(Taskbar);
