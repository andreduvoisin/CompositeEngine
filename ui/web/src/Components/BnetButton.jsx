import styled from 'styled-components';

const Button = styled.button`
    font-size: 15px;
    display: inline-block;
    position: relative;
    background-color: #0e86ca;
    border: solid 1px #00aeff;
    color: white;
    text-decoration: none;
    text-align: center;
    cursor: pointer;
    transition: color .2s,background-color .2s,border-color .2s;
    transition-property: color, background-color, border-color;
    transition-duration: 0.2s, 0.2s, 0.2s;
    transition-timing-function: ease, ease, ease;
    transition-delay: 0s, 0s, 0s;
    font-size: 15px;
    font-weight: 400;
    padding: 7px 42px;

    &:hover {
        background-color: #00aeff;
        border: solid 1px #00aeff;
        color: #fff;
    }
`;

export default Button;
