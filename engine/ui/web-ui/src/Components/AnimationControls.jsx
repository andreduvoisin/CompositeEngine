import React from 'react';
import styled from 'styled-components';
import classNames from 'classnames';
import './AnimationControls.less';

const AnimationControls = styled.ul`
  text-align: center;
  margin: 0;
  padding: 0;
    cursor: pointer;
`;

const AnimationControlsItem = styled.li`
  display: inline-block;
  border: solid 1px ${props => props.theme.colors.borders.button};
  border-radius: 3px;
  padding: 3px;
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

export default (props) => {
  const playButtonClasses = classNames('AnimationControls-playButton fa', {
    'fa-stop': props.isPlaying,
    'fa-play': !props.isPlaying
  });
  return (
    <AnimationControls className={classNames('AnimationControls', {
      'is-playing': props.isPlaying,
      'is-paused': !props.isPlaying
    })}>
      <AnimationControlsItem onClick={props.toggleAnimation}>
        <AnimationControlsIcon
          className={playButtonClasses}
        />
      </AnimationControlsItem>
    </AnimationControls>
  );
};
