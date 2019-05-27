import React from 'react';

import { storiesOf } from '@storybook/react';
import { State, Store } from "@sambego/storybook-state";

import AnimationControls from '../AnimationControls/AnimationControls';
import Toolbar from '../Toolbar/Toolbar';
import Layout from '../Layout/Layout';

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
