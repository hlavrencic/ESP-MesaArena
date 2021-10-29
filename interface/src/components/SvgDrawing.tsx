import React from 'react'
import { useSvgDrawing } from 'react-hooks-svgdrawing'

const Drawing = (props: {onDataChange?: (data: string | null) => void}) => {
  const [
    renderRef,
    action
  ] = useSvgDrawing();

  return (<div ref={renderRef} style ={{'height': '500px'}} onClick={e => {
    let data = action.getSvgXML();

    props.onDataChange && props.onDataChange(data);
  }} />);
};

export default Drawing;