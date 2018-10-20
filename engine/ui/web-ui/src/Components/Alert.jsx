import React from 'react';
import styled from 'styled-components';

const Container = styled.div`
    background: linear-gradient(#1c2534, #161a26);
    border: 1px solid rgba(255, 255, 255, .15);
    box-shadow: 1px 2px rgba(0, 0, 0, .5);
    position: relative;
    font-family: 'Encode Sans', sans-serif;
    color: white;
    margin: 9px;
`;

const InnerContainer = styled.div`
    padding: 16px;
`;

const Title = styled.h3`
    color: rgba(255, 255, 255, .5);
    text-transform: uppercase;
    font-size: 11px;
    margin: 0;
`;

const Body = styled.div`
    color: rgba(255,255,255,.8);
    margin: 10px 0 18px 0;
`;

export default (props) => {
    return (
        <Container>
            <InnerContainer>
                <Title>{props.title}</Title>
                <Body>{props.body}</Body>
                {props.children}
            </InnerContainer>
        </Container>
    );
};
