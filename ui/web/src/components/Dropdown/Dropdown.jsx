import React from 'react';

import {
  Downshift,
  DownshiftButton,
  DownshiftItem,
  withDownshift
} from './downshift-hoc';

import {
  Menu,
  ControllerButton,
  Value,
  Item,
  ArrowIcon
} from './styled';

const MyDownshiftButton = withDownshift(({ downshift: { isOpen } }) => {
  return (
    <DownshiftButton component={ControllerButton}>
      <ArrowIcon isOpen={isOpen} />
    </DownshiftButton>
  );
});

const MyDownShiftValue = withDownshift(
  ({ downshift: { isOpen, inputValue } }) => {
    return <Value isOpen={isOpen}>{inputValue || 'Select a item...'}</Value>;
  }
);

const MyDownshiftMenu = withDownshift(
  ({ downshift: { isOpen, highlightedIndex, selectedItem, itemToString, items } }) => {

    return (
      <Menu isOpen={isOpen}>
        {items.map((item, index) => {
          return isOpen ? (
            <DownshiftItem
              component={Item}
              key={item.id}
              item={item}
              index={index}
              isActive={highlightedIndex === index}
              isSelected={selectedItem.id === item.id}
            >
              {itemToString(item)}
            </DownshiftItem>
          ) : null;
        })}
      </Menu>
    );
  }
);

function Dropdown({
  width,
  items,
  onChange,
  selectedItem,
  ...rest
}) {
  if (!selectedItem) {
    selectedItem = items[0];
  }
  return (
    <Downshift
      onChange={onChange}
      itemToString={(item) => (item ? item.name : '')}
      items={items}
      initialSelectedItem={selectedItem}
    >
      <div
        style={rest.style || {}}
        css={{
          width: width,
          position: 'relative'
        }}
      >
        <MyDownShiftValue />
        <MyDownshiftButton />
        <MyDownshiftMenu />
      </div>
    </Downshift>
  );
}

Dropdown.defaultProps = {
  width: 250,
  items: [],
  onChange: (selectedItem) => {
    console.log('New selected item', selectedItem);
  }
};

export default Dropdown;
