import React from 'react';
import styled from 'styled-components';
import classNames from 'classnames';
import './AnimationControls.less';

const AnimationControlsList = styled.ul`
  text-align: center;
  margin: 0;
  padding: 0;
    cursor: pointer;
`;

const AnimationControlsListItem = styled.li`
  display: inline-block;
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
      <AnimationControlsListItem
        onClick={(props.isPlaying) ? null : props.toggleAnimation}
        className={classNames("AnimationControlsListItem-play", {
          "is-active": props.isPlaying
        })}
      >
        <AnimationControlsIcon
          className={classNames('fa fa-play')}
        />
      </AnimationControlsListItem>
    );
  }

  renderStopButton() {
    const { props } = this;
    return (
      <AnimationControlsListItem
        onClick={(props.isPlaying) ? props.toggleAnimation : null}
        className={classNames("AnimationControlsListItem-stop", {
          "is-active": !props.isPlaying
        })}
      >
        <AnimationControlsIcon
          className={classNames('fa fa-stop')}
        />
      </AnimationControlsListItem>
    );
  }

  render() {
    const { props } = this;
    return (
      <AnimationControlsList className={classNames('AnimationControls', {
        'is-playing': props.isPlaying,
        'is-paused': !props.isPlaying
      })}>
        {this.renderPlayButton()}
        {this.renderStopButton()}
      </AnimationControlsList>
    );
  }
}
