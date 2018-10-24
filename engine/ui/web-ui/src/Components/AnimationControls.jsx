import React from 'react';
import styled from 'styled-components';
import classNames from 'classnames';

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

export default (props) => {
  const playButtonClasses = classNames('fa', {
    'fa-pause': !props.isPlaying,
    'fa-play': props.isPlaying
  });
  return (
    <PlayControlsList>
      <PlayControlsListItem>
        <AnimationControlButton
          className={playButtonClasses}
          onClick={props.toggleAnimation}
        />
      </PlayControlsListItem>
    </PlayControlsList>
  );
};
