import ReactDOM from 'react-dom';

const Portal = ({
  children
}) => {
  const domNode = document.querySelector('#root');
  return ReactDOM.createPortal(
    children,
    domNode
  );
};

export default Portal;
