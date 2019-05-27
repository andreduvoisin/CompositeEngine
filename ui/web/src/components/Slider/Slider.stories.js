import React from 'react';

import { storiesOf } from '@storybook/react';
import { action } from '@storybook/addon-actions';

import Slider from './Slider.jsx';
import Toolbar from '../Toolbar/Toolbar';

storiesOf('Slider', module)
  .add('Default', () => {
    return (
      <Toolbar>
        <Slider></Slider>
      </Toolbar>
    );
  })
  .add('With Step', () => {
    return (
      <Toolbar>
        <Slider
          min={0}
          max={100}
          step={25} />
      </Toolbar>
    );
  })
  .add('With change handler', () => {
    return (
      <Toolbar>
        <Slider
          min={0}
          max={100}
          onChange={() => {
            action('slider change');
          }}
        />
      </Toolbar>
    );
  });
