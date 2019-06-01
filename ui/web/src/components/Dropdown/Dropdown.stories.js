import React from 'react';
import starWarsNames from 'starwars-names';

import { storiesOf } from '@storybook/react';

import Dropdown from './Dropdown.jsx';

const allItems = starWarsNames.all.map((s) => {
  return { name: s, id: s.toLowerCase() };
});

storiesOf('Dropdown', module).add('Default', () => {
  return <Dropdown width={175} items={allItems} />;
});
