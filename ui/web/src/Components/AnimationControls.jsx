import React from 'react';
import styled from 'styled-components';
import classNames from 'classnames';
import './AnimationControls.less';
import Slider from "./Slider";

const AnimationControlsList = styled.ul`
  text-align: center;
  margin: 0;
  padding: 0;
  cursor: pointer;
`;

const AnimationControlsListItem = styled.li`
  display: inline-block;
`;

const AnimationControlsButton = styled.div`
  border: solid 1px ${props => props.theme.colors.borders.button};
  border-radius: 3px;
  padding: 3px;
  margin-right: 3px;
  width: 20px;
  &:hover {
    background-color: #464a4d;
    border-color: #757b80;
  }
`;

const AnimationControlsFps = styled.div`
  border: solid 1px ${props => props.theme.colors.borders.button};
  border-radius: 3px;
  padding: 3px;
  margin-right: 3px;
`;

const AnimationControlsIcon = styled.i`
  margin-right: auto;
  margin-left: auto;
  display: inline-block;
  text-shadow: 1px 1px 3px ${props => props.theme.colors.transparentBlack};
`;

export default class AnimationControls extends React.Component {

  renderPlayButton() {
    const { props } = this;
    return (
      <AnimationControlsButton
        onClick={(props.isPlaying) ? null : props.toggleAnimation}
        className={classNames("AnimationControlsListItem-play", {
          "is-active": props.isPlaying
        })}
      >
        <AnimationControlsIcon
          className={classNames('fa fa-play')}
        />
      </AnimationControlsButton>
    );
  }

  renderStopButton() {
    const { props } = this;
    return (
      <AnimationControlsButton
        onClick={(props.isPlaying) ? props.toggleAnimation : null}
        className={classNames("AnimationControlsListItem-stop", {
          "is-active": !props.isPlaying
        })}
      >
        <AnimationControlsIcon
          className={classNames('fa fa-stop')}
        />
      </AnimationControlsButton>
    );
  }

  render() {
    const { props } = this;
    return (
      <AnimationControlsList className={classNames('AnimationControls', {
        'is-playing': props.isPlaying,
        'is-paused': !props.isPlaying
      })}>
        <AnimationControlsListItem>
          <AnimationControlsFps>
            <span>{props.animationState.fps}</span><span style={{ marginLeft: '3px' }}>FPS</span>
          </AnimationControlsFps>
        </AnimationControlsListItem>
        <AnimationControlsListItem>
          {this.renderPlayButton()}
        </AnimationControlsListItem>
        <AnimationControlsListItem>
          {this.renderStopButton()}
        </AnimationControlsListItem>
        <AnimationControlsListItem>
          <Slider
            step={0.0000000001}
            min={0}
            max={props.duration}
            value={props.currentTime}
            readOnly={true}
            onChange={props.setAnimationTime}
          />
        </AnimationControlsListItem>
        <AnimationControlsListItem>
          <select
            defaultValue={0}
            value={props.renderMode}
            onChange={props.setRenderMode}
          >
            <option value={0}>Mesh</option>
            <option value={1}>Skeleton</option>
            <option value={2}>Mesh w/ Skeleton</option>
          </select>
        </AnimationControlsListItem>
      </AnimationControlsList>
    );
  }
}
