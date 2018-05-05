import React, { Component } from 'react';
import mqtt from 'mqtt';
import _ from 'lodash';
import './App.css';

const MQTT_TOPIC_OUT = "esp/json"
const MQTT_TOPIC_IN  = "esp/json/set"

class App extends Component {

  constructor(props) {
    super(props);

    const client = mqtt.connect("ws://broker.mqttdashboard.com:8000/mqtt")

    client.on('connect',  this.onConnect)
    client.on('message',  this.onMessageReceived)

    this.state = {
      connected: false,
      brightness: undefined,
      messages: [],
      client,
    };
  }

  componentWillUnmount() {
    this.state.client.end()
  }

  componentDidMount() {
    this.setBrightness = this.setBrightness.bind(this)
    this.onBrightnessChange = this.onBrightnessChange.bind(this)
    this.updateBrightness = _.throttle(this.updateBrightness, 20);
  }



  onConnect = () => {
    const { client } = this.state;

    client.on('reconnect', () => {
      client.publish(MQTT_TOPIC_IN, JSON.stringify({ getState: "" }), { qos: 1 })
    })

    // console.log(require('util').inspect(client, { depth: null }));
    
    this.setState({ connected: true })

    client.subscribe(MQTT_TOPIC_IN)
    client.publish(MQTT_TOPIC_IN, JSON.stringify({ getState: "" }), { qos: 1 })
    client.subscribe(MQTT_TOPIC_OUT);
  };

  onConnectionLost = responseObject => {
    if (responseObject.errorCode !== 0) {
      console.log(`connection lost: ${responseObject.errorMessage}`);
    }
  };

  onMessageReceived = (topic, message, packet) => {
    const from = topic.toString()
    const body = JSON.parse(message.toString())

    switch (from) {
      case MQTT_TOPIC_OUT:
        return this.setBrightness(body)
        break;
      case MQTT_TOPIC_IN:
        console.log(body);
        break;
      default:
        return;
    }
  };

  setBrightness(level) {
    const { client } = this.state;
    if (level !== null && typeof level === 'object' && level.hasOwnProperty('brightness')) {
      console.log(`Syncing: ${level.brightness}`)
      this.setState({ brightness: level.brightness })
    } else if (level !== null && typeof level === 'number') {
      client.publish(MQTT_TOPIC_IN, JSON.stringify({ brightness: level }))
    }
  }

  updateBrightness(level) {
    this.setBrightness(level)
  }

  onBrightnessChange(event) {
    const level = Math.floor(event.target.value)
    this.setState({brightness: level});
    this.updateBrightness(level);
  }

  render() {
    const { connected, brightness } = this.state
    return (
      <div className="App">
        <div className="SliderBox">
          { connected && brightness !== undefined ? (
            <div>
              <h1 className="Level">{brightness}</h1>
              <input className="Slider" type="range" min="0" max="255" value={brightness} onChange={this.onBrightnessChange} />
            </div>
          ) : <h1 className="Status">Connecting ... </h1> }
        </div>
      </div>
    );
  }
}

export default App;
