export interface LightState {
  led_on: boolean;
  brightness: number;
}

export interface LightMqttSettings { 
  unique_id : string;
  name: string;
  mqtt_path : string;
}
