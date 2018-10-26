import { action } from '@storybook/addon-actions';
import { linkTo } from '@storybook/addon-links';
import { storiesOf } from '@storybook/react';
import { State, Store } from "@sambego/storybook-state";
import { Welcome } from '@storybook/react/demo';
import 'font-awesome/css/font-awesome.css';
import React from 'react';
import AnimationControls from '../src/Components/AnimationControls';
import BnetAlert from '../src/Components/BnetAlert';
import Slider from '../src/Components/Slider';
import Taskbar from '../src/Components/Taskbar';
import Layout from '../src/Components/Layout';

storiesOf('Welcome', module).add('to Storybook', () => <Welcome showApp={linkTo('Button')} />);

storiesOf('BnetAlert', module)
  .add('Default', () => {
    return (
      <BnetAlert title={'Attention'}>
        <span>Proactively brand interactive convergence without.</span>
      </BnetAlert>
    );
  });

storiesOf('Taskbar', module)
  .add('Default', () => {
    return (
      <Taskbar></Taskbar>
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
        <Taskbar>
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
        </Taskbar>
      </React.Fragment>
    );
  });

storiesOf('Slider', module)
  .add('Default', () => {
    return (
      <Taskbar>
        <Slider></Slider>
      </Taskbar>
    );
  })
  .add('With Step', () => {
    return (
      <Taskbar>
        <Slider
          min={0}
          max={100}
          step={25} />
      </Taskbar>
    );
  })
  .add('With change handler', () => {
    return (
      <Taskbar>
        <Slider
          min={0}
          max={100}
          onChange={() => {
            action('slider change');
          }}
        />
      </Taskbar>
    );
  });
