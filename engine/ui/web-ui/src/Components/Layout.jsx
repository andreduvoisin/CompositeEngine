import React from 'react';
import styled from 'styled-components';

const Container = styled.div`
  display: flex;
  justify-content: space-between;
  align-items: center;
  height: 100vh;
`;

const Sidebar = styled.div`
  flex: 0 0 auto; /* Forces side columns to stay same width */
  width: 280px; /* Sets the width of side columns */
  height: 100%;
`;

const Center = styled.div`
  flex:1 1 auto;
  height: 100%;
`;

export default {
  Container,
  Sidebar,
  Center
};
