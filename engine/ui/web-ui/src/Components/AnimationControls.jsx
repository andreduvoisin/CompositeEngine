import React from 'react';
import styled from 'styled-components';
import classNames from 'classnames';
import './AnimationControls.less';

const AnimationControls = styled.ul`
  text-align: center;
  margin: 0;
  padding: 0;
`;

const AnimationControlsItem = styled.li`
  display: inline-block;
`;

const AnimationControlsButton = styled.i`
  cursor: pointer;
  margin-right: auto;
  margin-left: auto;
  display: inline-block;
  width: 10px;
  margin-right: 10px;
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
      <AnimationControlsItem>
        <AnimationControlsButton
          className={playButtonClasses}
          onClick={props.toggleAnimation}
        />
      </AnimationControlsItem>
    </AnimationControls>
  );
};
