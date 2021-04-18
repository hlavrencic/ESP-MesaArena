import React, { ChangeEvent, Component } from "react";
import {Button, Container, Grid, LinearProgress, Box, TextField  } from "@material-ui/core";
import { SvgService, Dimensions } from './SvgService';

export interface SVGComponentProps {
  arduinoPosition: Dimensions | undefined;
  onUpdate(nextPosition: Dimensions | undefined): void;
}

interface SVGComponentModel {
  progress: number;
}

export default class SvgComponent extends Component<SVGComponentProps, SVGComponentModel> {
    private svgService: SvgService | undefined;
    constructor(props: SVGComponentProps) {
        super(props);

        this.state = {progress: 1};
    }

  componentDidUpdate(prevProps: SVGComponentProps) {
    let nextProps = this.props;
        
    if(!this.svgService) return;
    if(!this.svgService.scaledPoint) {
      this.moveNext();
      return;
    }

    if(!nextProps.arduinoPosition) return;

    if(this.svgService.scaledPoint.x !== nextProps.arduinoPosition.x ){
      return;
    }

    if(this.svgService.scaledPoint.y !== nextProps.arduinoPosition.y ){
      return;
    }

    setTimeout(() => this.moveNext(), 300);
  }

  onFileChange(ev: React.ChangeEvent<HTMLInputElement>) {
    let self = this;

    if(!ev.target.files){
        return;
    }

    let file = ev.target.files[0];
    this.setState({progress: 0 });

    try{
        let svgContainer = document.getElementById("svgContainer");
        if(!svgContainer){
            throw new Error("svgContainer not found");
        }

        SvgService
        .load(file, svgContainer)
        .then(svgService => {
            if(!svgService) throw new Error('svgService undefined');
            svgService.configure({x: 4000, y:35000}, null);
            self.svgService = svgService;
        });
    } catch(e){
      alert(e);
    }
  }

  async moveNext() {
    if(!this.svgService) return;

    this.svgService.moveNext();
    let progress = this.svgService.moment * 100 / this.svgService.maxMoment;
    this.setState({ progress });
    
    this.props.onUpdate && this.props.onUpdate(this.svgService.scaledPoint);
  }

  render() {
    


    return (
      <div>
        <Grid item xs={12} id="svgContainer" />
        
        <div>
          <TextField type="file" onChange={this.onFileChange.bind(this)} />
        </div>

        <LinearProgress variant="determinate" value={this.state.progress} />
        <Button onClick={this.moveNext.bind(this)}>MOVE</Button>
      </div>
    );
  }
}