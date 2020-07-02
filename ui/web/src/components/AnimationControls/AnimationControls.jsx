import React from 'react';
import styled from 'styled-components';
import classNames from 'classnames';
import Slider from '../Slider/Slider';
import Dropdown from '../Dropdown/Dropdown';

import './AnimationControls.less';

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
  border: solid 1px ${(props) => props.theme.colors.borders.button};
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
  text-shadow: 1px 1px 3px ${(props) => props.theme.colors.transparentBlack};
`;

const PlayButton = (props) => {
  return (
    <AnimationControlsButton
      onClick={props.isPlaying ? null : props.toggleAnimation}
      className={classNames('AnimationControlsListItem-play', {
        'is-active': props.isPlaying
      })}
    >
      <AnimationControlsIcon className={classNames('fa fa-play')} />
    </AnimationControlsButton>
  );
};

const StopButton = (props) => {
  return (
    <AnimationControlsButton
      onClick={props.isPlaying ? props.toggleAnimation : null}
      className={classNames('AnimationControlsListItem-stop', {
        'is-active': !props.isPlaying
      })}
    >
      <AnimationControlsIcon className={classNames('fa fa-stop')} />
    </AnimationControlsButton>
  );
};

const AnimationControls = (props) => {

  const renderModeMenuItems = [
    {
      name: 'Mesh',
      id: 'mesh',
      value: 0
    },
    {
      name: 'Skeleton',
      id: 'skeleton',
      value: 1
    },
    {
      name: 'Mesh w/ Skeleton',
      id: 'mesh-w-skeleton',
      value: 2
    },
    {
      name: 'Mesh w/ Wireframe',
      id: 'mesh-w-wireframe',
      value: 3
    }
  ];

  return (
    <AnimationControlsList
      className={classNames('AnimationControls', {
        'is-playing': props.isPlaying,
        'is-paused': !props.isPlaying
      })}
    >
      <AnimationControlsListItem>
        <PlayButton {...props} />
      </AnimationControlsListItem>
      <AnimationControlsListItem>
        <StopButton {...props} />
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
        <Dropdown
          style={{
            marginLeft: '10px'
          }}
          width={150}
          items={renderModeMenuItems}
          onChange={(selectedItem) => {
            props.setRenderMode(selectedItem.value);
          }}
        />
      </AnimationControlsListItem>
    </AnimationControlsList>
  );
};

export default AnimationControls;
