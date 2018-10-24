import React from 'react';
import styled from 'styled-components';

const Container = styled.div`
  background: ${props => props.theme.backgrounds.default};
  border-bottom: solid 1px ${props => props.theme.colors.borders.bottom};
  border-top: solid 1px ${props => props.theme.colors.borders.top};
  color: ${props => props.theme.colors.text};
  height: 26px;
  font-size: 10px;
  display: flex;
  align-items: center;
  justify-content: center;
`;

const InnerContainer = styled.div`
  padding: 7px;
`;

const Taskbar = (props) => {
  return (
    <Container>
      <InnerContainer>
        {(props.children) ? props.children : null}
      </InnerContainer >
    </Container >
  );
};

export default Taskbar;
