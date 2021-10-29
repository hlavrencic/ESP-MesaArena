import { Button } from '@material-ui/core';
import React, { Component } from 'react';
import Drawing from '../components/SvgDrawing';

class DemoInformation extends Component {
  
  
  render() {
    return (
      <>
      <Drawing onDataChange={data => console.log(data)}/>
      <Button onClick={e => {}}>SEND</Button>
      </>
    )
  }

}

export default DemoInformation;
