import { Button } from '@material-ui/core';
import React from 'react'
import { useSvgDrawing } from 'react-hooks-svgdrawing'

const Drawing = (props: {onDataChange?: (data: string | null) => void}) => {
  const [
    renderRef,
    action
  ] = useSvgDrawing();

  return (
  <>
  <div ref={renderRef} style ={{'height': '500px'}} />
  <Button onClick={e => {
        let data = action.getSvgXML();

        props.onDataChange && props.onDataChange(data);
  }}>SEND</Button>
  </>);
};

export default Drawing;