import React from 'react';
import styled from '@emotion/styled';

const Item = styled('li')(
  {
    position: 'relative',
    cursor: 'pointer',
    display: 'block',
    border: 'none',
    height: 'auto',
    textAlign: 'left',
    borderTop: 'none',
    lineHeight: '1em',
    color: 'rgba(187,187,187,0.7)',
    fontSize: 10,
    textTransform: 'none',
    fontWeight: '200',
    boxShadow: 'none',
    padding: '.2rem .4rem',
    whiteSpace: 'normal',
    wordWrap: 'normal',
  },
  ({ isActive, isSelected }) => {
    const styles = [];
    if (isActive) {
      styles.push({
        background: '#4B6EAF'
      });
    }
    if (isSelected) {
      styles.push({
        background: '#4B6EAF',
        fontWeight: '700'
      });
    }
    return styles;
  }
);
const onAttention = '&:hover, &:focus';
const Input = styled('input')(
  {
    width: '100%', // full width - icon width/2 - border
    fontSize: 14,
    wordWrap: 'break-word',
    lineHeight: '1em',
    outline: 0,
    whiteSpace: 'normal',
    minHeight: '1em',
    background: '#3C3F41',
    display: 'inline-block',
    padding: '4px 0px 4px 4px',
    color: '#BBBBBB',
    boxShadow: 'none',
    border: '1px solid #5E6060',
    borderRadius: '.30rem',
    transition: 'box-shadow .1s ease,width .1s ease',
    boxSizing: 'border-box',
    [onAttention]: {
      borderColor: '#96c8da',
      boxShadow: '0 2px 3px 0 #5E6060'
    }
  },
  ({ isOpen }) =>
    isOpen
      ? {
          borderBottomLeftRadius: '0',
          borderBottomRightRadius: '0',
          [onAttention]: {
            boxShadow: 'none'
          }
        }
      : null
);

const Value = styled('div')(
  {
    width: '100%', // full width - icon width/2 - border
    fontSize: 12,
    wordWrap: 'break-word',
    lineHeight: '1em',
    outline: 0,
    whiteSpace: 'normal',
    height: '24px',
    background: '#3C3F41',
    display: 'inline-block',
    padding: '4px 0px 4px 4px',
    color: 'rgba(187,187,187,0.7)',
    boxShadow: 'none',
    border: '1px solid #5E6060',
    borderRadius: '.30rem',
    transition: 'box-shadow .1s ease,width .1s ease',
    boxSizing: 'border-box',
    cursor: 'default',
    textAlign: 'left'
  },
  ({ isOpen }) =>
    isOpen
      ? {
          borderBottomLeftRadius: '0',
          borderBottomRightRadius: '0',
          [onAttention]: {
            boxShadow: 'none'
          }
        }
      : null
);

const Label = styled('label')({
  fontWeight: 'bold',
  display: 'block',
  marginBottom: 10
});

const BaseMenu = styled('ul')(
  {
    padding: 0,
    marginTop: 0,
    position: 'absolute',
    backgroundColor: '#3C3F41',
    width: '100%',
    maxHeight: '20rem',
    overflowY: 'auto',
    overflowX: 'hidden',
    outline: '0',
    transition: 'opacity .1s ease',
    borderRadius: '0 0 .28571429rem .28571429rem',
    boxShadow: '0 2px 3px 0 rgba(34,36,38,.15)',
    boxSizing: 'border-box',
    borderColor: '#5E6060',
    borderTopWidth: '0',
    borderRightWidth: 1,
    borderBottomWidth: 1,
    borderLeftWidth: 1,
    borderStyle: 'solid',
  },
  ({ isOpen }) => ({
    border: isOpen ? null : 'none'
  })
);

const Menu = React.forwardRef((props, ref) => (
  <BaseMenu innerRef={ref} {...props} />
));

const ControllerButton = styled('button')({
  backgroundColor: 'transparent',
  border: 'none',
  position: 'absolute',
  right: 0,
  top: 0,
  cursor: 'pointer',
  width: 28,
  height: '100%',
  boxSizing: 'border-box',
});

function ArrowIcon({ isOpen }) {
  return (
    <svg
      viewBox="0 0 20 20"
      preserveAspectRatio="none"
      width={10}
      fill="transparent"
      stroke="#979797"
      strokeWidth="1.1px"
      transform={isOpen ? 'rotate(180)' : undefined}
    >
      <path d="M1,6 L10,15 L19,6" />
    </svg>
  );
}

function XIcon() {
  return (
    <svg
      viewBox="0 0 20 20"
      preserveAspectRatio="none"
      width={12}
      fill="transparent"
      stroke="#979797"
      strokeWidth="1.1px"
    >
      <path d="M1,1 L19,19" />
      <path d="M19,1 L1,19" />
    </svg>
  );
}

export {
  Menu,
  ControllerButton,
  Input,
  Item,
  Value,
  ArrowIcon,
  XIcon,
  Label
};
