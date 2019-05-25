import React from 'react';
import styled from 'styled-components';
import Portal from '../Components/Portal';
import withFps from './withFps';

const Container = styled.div`
  position: absolute;
  bottom: 10px;
  right: 10px;
  color: white;
  padding: 3px;
  border-radius: 4px;
  border: solid 1px rgba(255, 255, 255, 0.5);
  width: 30px;
  text-align: center;
  background-color: rgba(0, 0, 0, 0.5);
`;

const FpsCounter = withFps(({ fps }) => {
  return (
    <Portal>
      <Container>{fps}</Container>
    </Portal>
  );
});

export default FpsCounter;
