import React from 'react';
import styled from 'styled-components';

const Container = styled.div`
  background: ${props => props.theme.backgrounds.default};
  border-bottom: solid 1px ${props => props.theme.colors.borders.bottom};
  border-top: solid 1px ${props => props.theme.colors.borders.top};
  color: ${props => props.theme.colors.text};
  font-size: 10px;
  padding: 7px;
`;

const Toolbar = (props) => {
  return (
    <Container>
      {(props.children) ? props.children : null}
    </Container >
  );
};

export default Toolbar;
