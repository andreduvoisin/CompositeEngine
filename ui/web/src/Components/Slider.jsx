import React from 'react';
import './Slider.less';

export default (props) => {
  return (
    <input
      type="range"
      min={props.min}
      max={props.max}
      step={props.step}
      value={props.value}
      onChange={props.onChange}
    />
  );
};
