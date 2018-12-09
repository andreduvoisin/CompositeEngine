import React from 'react';
import styled from 'styled-components';

const decorateFullHeight = (WrappedComponent) => {
  return class FullHeight extends React.Component {
    render() {
      const FullHeightComponent = styled(WrappedComponent)`
        height: 100vh;
      `;
      return <FullHeightComponent {...this.props} />;
    }
  }
};

const Container = styled.div`
  display: flex;
  justify-content: space-between;
  align-items: center;
`;

const FullHeightContainer = decorateFullHeight(Container);

const Sidebar = styled.div`
  flex: 0 0 auto; /* Forces side columns to stay same width */
  width: ${props => (props.width) ? props.width : 280}px; /* Sets the width of side columns */
  height: 100%;
`;

const Center = styled.div`
  flex:1 1 auto;
  height: 100%;
`;

export default {
  decorateFullHeight,
  FullHeightContainer,
  Container,
  Sidebar,
  Center
};
