import { createGlobalStyle } from 'styled-components';

const GlobalStyles = createGlobalStyle`
  body {
    font-family: ${props => props.theme.fonts.family};
    background-color: ${props => props.theme.colors.transparentBlack};
    height: 100%;
    overflow: hidden;  /*makes the body non-scrollable (we will add scrolling to the sidebar and main content containers)*/
    margin: 0px;  /*removes default style*/
    box-sizing: border-box;
  }
  #root {
    height: 100vh;
  }
  img {
    max-width: 100%;
  }
`;

export default GlobalStyles;
