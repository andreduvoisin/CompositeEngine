import { action } from '@storybook/addon-actions';
import { linkTo } from '@storybook/addon-links';
import { storiesOf } from '@storybook/react';
import { Welcome } from '@storybook/react/demo';
import 'font-awesome/css/font-awesome.css';
import React from 'react';
import AnimationControls from '../src/Components/AnimationControls';
import BnetAlert from '../src/Components/BnetAlert';
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
    return (
      <React.Fragment>
        <Taskbar>
          <Layout.Container>
            <Layout.Center>
              <AnimationControls isPlaying={false} toggleAnimation={action('toggleAnimation')} />
            </Layout.Center>
          </Layout.Container>
        </Taskbar>
        <Taskbar>
          <Layout.Container>
            <Layout.Center>
              <AnimationControls isPlaying={true} toggleAnimation={action('toggleAnimation')} />
            </Layout.Center>
          </Layout.Container>
        </Taskbar>
      </React.Fragment>
    );
  });
