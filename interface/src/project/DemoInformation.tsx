import { Button, Grid } from '@material-ui/core';
import React, { Component } from 'react';
import { GO_TO_ENDPOINT } from '../api';
import { restController, RestControllerProps } from '../components';
import Drawing from '../components/SvgDrawing';
import { SvgService, DimensionsDataController } from '../components/SvgService';

type NextPositionControllerProps = RestControllerProps<DimensionsDataController>;

class DemoInformation extends Component<NextPositionControllerProps> {
  
  svgService: SvgService | null = null;
  
  componentDidMount() {
    this.props.loadData();
  }

  moveNext(){

    let iterations = this.svgService?.moveNext();

    if(iterations == 0 || !this.svgService?.scaledPoint) {
      return;
    }

    this.props.setData(this.svgService?.scaledPoint, this.props.saveData);
    
  }

  render() {
    const { data } = this.props

    if(this.svgService?.scaledPoint && data){
      if(this.svgService.scaledPoint.x === data.xActual && 
        this.svgService.scaledPoint.y === data.yActual){
          this.moveNext();
      } else {
          this.props.loadData();
      }
    }

    return (
      <>
      <Drawing onDataChange={inputData => {
        if(!inputData){
          return;
        }

        let svgContainer = document.getElementById("svgContainer");
        if(!svgContainer){
            throw new Error("svgContainer not found");
        }

        console.log(inputData);
        this.svgService = SvgService.loadContent(inputData as string, svgContainer);
        this.svgService.configure({x: 4000, y:35000}, null);
      }}/>
      <Grid item xs={12} id="svgContainer" />
      <Button onClick={e => {
        this.moveNext();
        }}>SEND</Button>
      </>
    )
  }

}

export default restController(GO_TO_ENDPOINT, DemoInformation, false);
