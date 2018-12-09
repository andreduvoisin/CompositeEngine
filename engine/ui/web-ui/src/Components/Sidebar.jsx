import React from 'react';
import styled from 'styled-components';

const Container = styled.div`
  background: ${props => props.theme.backgrounds.default};
  border-top: solid 1px ${props => props.theme.colors.borders.top};
  border-right: solid 1px ${props => props.theme.colors.borders.bottom};
  box-shadow: 1px 2px rgba(0, 0, 0, .5);
  flex: 1;
  height: 100%;
  position: relative;

  &::before {
    content: "";
    position: absolute;
    height: 100%;
    background: red;
    top: 0;
    bottom: 0;
    width: 1px;
    right: -1px;
    z-index: 999;
    background-color: ${props => props.theme.colors.borders.top};
  }
`;

export default (props) => {
  return (
    <Container>
      {props.children}
    </Container>
  );
};
