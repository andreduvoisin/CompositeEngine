import React from 'react';
import { useSelector } from 'react-redux';
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

const PlayButton = ({
  onClick
}) => {
  const animationStatus = useSelector(state => state.animationState.animationStatus);
  return (
    <AnimationControlsButton
      onClick={animationStatus === 'PAUSED' ? onClick : null}
      className={classNames('AnimationControlsListItem-play', {
        'is-active': animationStatus === 'PLAYING'
      })}
    >
      <AnimationControlsIcon className={'fa fa-play'} />
    </AnimationControlsButton>
  );
};

const StopButton = ({
  onClick
}) => {
  const animationStatus = useSelector(state => state.animationState.animationStatus);
  return (
    <AnimationControlsButton
      onClick={animationStatus === 'PLAYING' ? onClick : null}
      className={classNames('AnimationControlsListItem-stop', {
        'is-active': animationStatus === 'PAUSED'
      })}
    >
      <AnimationControlsIcon className={'fa fa-stop'} />
    </AnimationControlsButton>
  );
};

const AnimationScrubber = ({
  onChange
}) => {
  const duration = useSelector(state => state.animationState.duration);
  const currentTime = useSelector(state => state.animationState.currentTime);
  return (
    <Slider
      step={0.0000000001}
      min={0}
      max={duration}
      value={currentTime}
      readOnly={true}
      onChange={onChange}
    />
  );
};

const AnimationControls = ({
  setAnimationTime,
  setRenderMode,
  toggleAnimation
}) => {

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

  const animationStatus = useSelector(state => state.animationState.animationStatus);

  return (
    <AnimationControlsList
      className={classNames('AnimationControls', {
        'is-playing': animationStatus === "PLAYING",
        'is-paused': animationStatus === "PAUSED",
      })}
    >
      <AnimationControlsListItem>
        <PlayButton onClick={toggleAnimation} />
      </AnimationControlsListItem>
      <AnimationControlsListItem>
        <StopButton onClick={toggleAnimation} />
      </AnimationControlsListItem>
      <AnimationControlsListItem>
        <AnimationScrubber onChange={setAnimationTime} />
      </AnimationControlsListItem>
      <AnimationControlsListItem>
        <Dropdown
          style={{
            marginLeft: '10px'
          }}
          width={150}
          items={renderModeMenuItems}
          onChange={(selectedItem) => {
            setRenderMode(selectedItem.value);
          }}
        />
      </AnimationControlsListItem>
    </AnimationControlsList>
  );
};

export default AnimationControls;
