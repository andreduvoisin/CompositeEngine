import { action } from '@storybook/addon-actions';
import { linkTo } from '@storybook/addon-links';
import { storiesOf } from '@storybook/react';
import { State, Store } from "@sambego/storybook-state";
import { Welcome } from '@storybook/react/demo';
import 'font-awesome/css/font-awesome.css';
import React from 'react';
import AnimationControls from '../src/Components/AnimationControls';
import Slider from '../src/Components/Slider';
import Toolbar from '../src/Components/Toolbar';
import Layout from '../src/Components/Layout';;

storiesOf('Welcome', module).add('to Storybook', () => <Welcome showApp={linkTo('Button')} />);

storiesOf('Toolbar', module)
  .add('Default', () => {
    return (
      <Toolbar></Toolbar>
    );
  })
  .add('With Animation Controls', () => {
    const store = new Store({
      isPlaying: true
    });
    const togglePlaying = () => {
      const toggled = !store.get('isPlaying');
      store.set({ isPlaying: toggled })
    };
    return (
      <React.Fragment>
        <Toolbar>
          <Layout.Container>
            <Layout.Center>
              <State store={store}>
                <AnimationControls
                  isPlaying={false}
                  toggleAnimation={togglePlaying}
                />
              </State>
            </Layout.Center>
          </Layout.Container>
        </Toolbar>
      </React.Fragment>
    );
  });

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
