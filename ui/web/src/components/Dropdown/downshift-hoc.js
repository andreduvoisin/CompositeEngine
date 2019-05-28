/**
 * Adapted from https://github.com/kentcdodds/downshift-examples
 */
import React from 'react';
import hoistNonReactStatics from 'hoist-non-react-statics';
import Downshift from 'downshift';

const DownshiftContext = React.createContext();

const DownshiftInput = withDownshift(
  ({ downshift: { getInputProps }, component: Comp = 'input', ...rest }) => (
    <Comp {...getInputProps(rest)} />
  )
);

const DownshiftLabel = withDownshift(
  ({ downshift: { getLabelProps }, component: Comp = 'label', ...rest }) => (
    <Comp {...getLabelProps(rest)} />
  )
);

const DownshiftButton = withDownshift(
  ({
    downshift: { getToggleButtonProps },
    component: Comp = 'button',
    ...rest
  }) => <Comp {...getToggleButtonProps(rest)} />
);

const DownshiftItem = withDownshift(
  ({ downshift: { getItemProps }, component: Comp = 'div', ...rest }) => (
    <Comp {...getItemProps(rest)} />
  )
);

function withDownshift(Component) {
  function Wrapper(props, ref) {
    return (
      <DownshiftContext.Consumer>
        {(downshiftContext) => (
          <Component downshift={downshiftContext} {...props} ref={ref} />
        )}
      </DownshiftContext.Consumer>
    );
  }

  Wrapper.displayName = `withDownshift(${Component.displayName ||
    Component.name})`;

  return hoistNonReactStatics(React.forwardRef(Wrapper), Component);
}

function DownshiftComps({ children, ...rest }) {
  return (
    <Downshift {...rest}>
      {(downshift) => {
        return (
          <div>
            <DownshiftContext.Provider value={{
              ...downshift,
              items: rest.items
            }}>
              {children}
            </DownshiftContext.Provider>
          </div>
        );
      }}
    </Downshift>
  );
}

export {
  withDownshift,
  DownshiftComps as Downshift,
  DownshiftLabel,
  DownshiftButton,
  DownshiftInput,
  DownshiftItem
};
