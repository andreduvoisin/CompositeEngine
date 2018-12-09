import { configure, addDecorator } from '@storybook/react';
import { withThemes } from 'storybook-styled-components';

import theme from '../src/theme';

const themes = {
  'Default': theme
};

addDecorator(withThemes(themes))

// automatically import all files ending in *.stories.js
const req = require.context('../stories', true, /.stories.js$/);
function loadStories() {
  req.keys().forEach(filename => req(filename));
}

configure(loadStories, module);
