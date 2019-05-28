import { css } from 'emotion';
import matchSorter from 'match-sorter';

function filterItems(items, filter) {
  return filter
    ? matchSorter(items, filter, {
        keys: ['name']
      })
    : items;
}

function getStringItems(items, filter) {
  return filterItems(items, filter).map(({ name }) => name);
}

const itemToString = (i) => (i ? i.name : '');

export { css, itemToString, filterItems, getStringItems };
