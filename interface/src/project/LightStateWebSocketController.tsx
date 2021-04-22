import React, { Component } from 'react';
import { ValidatorForm } from 'react-material-ui-form-validator';

import { Typography, Box, Switch, Slider, Input, Button } from '@material-ui/core';
import { WEB_SOCKET_ROOT } from '../api';
import { WebSocketControllerProps, WebSocketFormLoader, WebSocketFormProps, webSocketController } from '../components';
import { SectionContent, BlockFormControlLabel, SvgComponent } from '../components';

import { LightState } from './types';
import { makeStyles } from "@material-ui/core/styles";
import { Dimensions } from '../components/SvgService';

export const LIGHT_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "lightState";

type LightStateWebSocketControllerProps = WebSocketControllerProps<LightState>;

class LightStateWebSocketController extends Component<LightStateWebSocketControllerProps> {

  render() {
    return (
      <SectionContent title='WebSocket Controller' titleGutter>
        <WebSocketFormLoader
          {...this.props}
          render={props => (
            <LightStateWebSocketControllerForm {...props} />
          )}
        />
      </SectionContent>
    )
  }

}

export default webSocketController(LIGHT_SETTINGS_WEBSOCKET_URL, 100, LightStateWebSocketController);

type LightStateWebSocketControllerFormProps = WebSocketFormProps<LightState>;

function LightStateWebSocketControllerForm(props: LightStateWebSocketControllerFormProps) {
  const { data, saveData, setData } = props;

  const changeLedOn = (event: React.ChangeEvent<HTMLInputElement>) => {
    setData({ led_on: event.target.checked, brightness: data.brightness, arduinoPosition: data.arduinoPosition, nextPosition: data.nextPosition }, saveData);
  }

  const changeBrighness = (event: React.ChangeEvent<{}>, newValue: number | number[]) => {
    setData({ led_on: true, brightness: +newValue, arduinoPosition: data.arduinoPosition, nextPosition: data.nextPosition }, saveData);
    
  }

  const inputChangeX = (event: React.ChangeEvent<HTMLInputElement>) => {
    data.nextPosition = data.nextPosition ?? {x:0, y:0};
    data.nextPosition.x = +event.target.value;
    setData({led_on: data.led_on, brightness: data.brightness, arduinoPosition: data.arduinoPosition, nextPosition: data.nextPosition});
  }

  const inputChangeY = (event: React.ChangeEvent<HTMLInputElement>) => {
    data.nextPosition = data.nextPosition ?? {x:0, y:0};
    data.nextPosition.y = +event.target.value;
    setData({led_on: data.led_on, brightness: data.brightness, arduinoPosition: data.arduinoPosition, nextPosition: data.nextPosition});
  }

  const sendNextPosition = (nextPosition: Dimensions) => {
    setData({led_on: data.led_on, brightness: data.brightness, arduinoPosition: data.arduinoPosition, nextPosition: nextPosition}, saveData);
  }

  const useStyles = makeStyles({
    root: {
      width: 300
    }
  });

  const classes = useStyles();

  return (
    <ValidatorForm onSubmit={saveData}>
      <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
        <Typography variant="body1">
          The switch below controls the LED via the WebSocket. It will automatically update whenever the LED state changes.
        </Typography>
      </Box>
      <BlockFormControlLabel
        control={
          <Switch
            checked={data.led_on}
            onChange={changeLedOn}
            color="primary"
          />
        }
        label="LED State?"
      />
      <div className={classes.root}>
      <Slider
              value={data.brightness}
              onChange={changeBrighness}
              defaultValue={100}
              aria-labelledby="discrete-slider"
              valueLabelDisplay="auto"
              step={1}
              min={1}
              max={100}
            />
      </div>
      <SvgComponent onUpdate={sendNextPosition} arduinoPosition={data.arduinoPosition} />

      <BlockFormControlLabel
        control={
          <Input
            onChange={inputChangeX}
            color="primary"
          />
        }
        label="X"
      />
      <BlockFormControlLabel
        control={
          <Input
            onChange={inputChangeY}
            color="primary"
          />
        }
        label="Y"
      />
      <Button onClick={saveData} >Send</Button>
    </ValidatorForm>
  );
}
