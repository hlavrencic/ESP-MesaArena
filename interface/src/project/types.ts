import { Dimensions } from "../components/SvgService";

export interface LightState {
  led_on: boolean;
  brightness: number;
  arduinoPosition: Dimensions | undefined;
  nextPosition: Dimensions;
}

export interface LightMqttSettings { 
  unique_id : string;
  name: string;
  mqtt_path : string;
}
