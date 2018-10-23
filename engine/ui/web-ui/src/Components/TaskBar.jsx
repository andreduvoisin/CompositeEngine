import React from 'react';
import { connect } from 'react-redux';
import styled from 'styled-components';
import { resetAnimation, toggleAnimation } from "../redux/actions";
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

const AnimationControlButton = styled.i`
  cursor: pointer;
  margin-right: auto;
  margin-left: auto;
  display: inline-block;
  width: 10px;
  margin-right: 10px;
`;

const PlayControlsList = styled.ul`
  text-align: center;
  margin: 0;
  padding: 0;
`;

const PlayControlsListItem = styled.li`
  display: inline-block;
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
            <PlayControlsList>
              <PlayControlsListItem>
                {
                  (props.animationPlayingState === AnimationPlayingStates.PLAYING)
                    ? <AnimationControlButton className="fa fa-pause" onClick={props.toggleAnimation}></AnimationControlButton>
                    : <AnimationControlButton className="fa fa-play" onClick={props.toggleAnimation}></AnimationControlButton>
                }
              </PlayControlsListItem>
            </PlayControlsList>
          </Layout.Center>
          <Layout.Sidebar></Layout.Sidebar>
        </Layout.Container>
      </InnerContainer >
    </Container >
  );
};

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(Taskbar);
