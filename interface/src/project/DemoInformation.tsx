import { Button, Grid } from '@material-ui/core';
import React, { Component } from 'react';
import Drawing from '../components/SvgDrawing';
import { SvgService, Dimensions } from '../components/SvgService';

class DemoInformation extends Component {
  
  
  render() {
    return (
      <>
      <Drawing onDataChange={data => {
        if(!data){
          return;
        }

        let svgContainer = document.getElementById("svgContainer");
        if(!svgContainer){
            throw new Error("svgContainer not found");
        }


        console.log(data);
        let svgService = SvgService.loadContent(data as string, svgContainer);
        svgService.configure({x: 4000, y:35000}, null);
      }}/>
      <Grid item xs={12} id="svgContainer" />
      <Button onClick={e => {}}>SEND</Button>
      </>
    )
  }

}

export default DemoInformation;
