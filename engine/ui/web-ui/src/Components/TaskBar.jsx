import React from 'react';
import styled from 'styled-components';

const Container = styled.div`
    background: ${props => props.theme.shared.background};
    border: 1px solid rgba(255, 255, 255, .15);
    box-shadow: 1px 2px rgba(0, 0, 0, .5);
    font-family: 'Encode Sans', sans-serif;
    color: white;
`;

const InnerContainer = styled.div`
    padding: 16px;
`;

export default (props) => {
  return (
    <Container>
      <InnerContainer />
    </Container>
  );
};
