import React from 'react';
import styled from 'styled-components';

const Container = styled.div`
    background: ${props => props.theme.shared.background};
    border-right: 1px solid rgba(255, 255, 255, .15);
    border-left: 1px solid rgba(255, 255, 255, .15);
    box-shadow: 1px 2px rgba(0, 0, 0, .5);
    color: white;
    flex: 1;
    height: 100%;
`;

export default (props) => {
  return (
    <Container>
      {props.children}
    </Container>
  );
};
