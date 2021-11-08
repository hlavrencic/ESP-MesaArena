import { Button, Grid } from '@material-ui/core';
import React, { Component } from 'react';
import { GO_TO_ENDPOINT } from '../api';
import { restController, RestControllerProps } from '../components';
import Drawing from '../components/SvgDrawing';
import { SvgService, Dimensions } from '../components/SvgService';

type NextPositionControllerProps = RestControllerProps<Dimensions>;

class DemoInformation extends Component<NextPositionControllerProps> {
  
  svgService: SvgService | null = null;
  
  componentDidMount() {
    this.props.loadData();
  }

  render() {
    const { data, setData, saveData } = this.props

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
        this.svgService?.moveNext();

        if(!this.svgService?.scaledPoint) {
          return;
        }

        setData(this.svgService?.scaledPoint, saveData);
        }}>SEND</Button>
      </>
    )
  }

}

export default restController(GO_TO_ENDPOINT, DemoInformation, false);
