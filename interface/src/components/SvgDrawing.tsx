import React from 'react'
import { useSvgDrawing } from 'react-hooks-svgdrawing'

const Drawing = () => {
  const [
    renderRef,
    action
  ] = useSvgDrawing()
  return <div ref={renderRef} style ={{'height': '500px'}} />
};

export default Drawing;