import { json } from "body-parser";

const Datastore = require('@google-cloud/datastore');
const datastore = Datastore();
const kind = 'RoomDataPoint';

export interface RoomDataPoint {
    temperature: number,
    humidity: number,
    ambientLight: number,
    timestamp: string
    cpuTemp: string
}

// Structure of the pubsub message for needed fields 
export interface RaspberryPiRoomMonitoringPubSubMessage {  
    data:{  
       data: string; // base 64 encoded string
    }
 }

export function RaspberryPiRoomMonitoringPubSubHandler(req: RaspberryPiRoomMonitoringPubSubMessage, callback: Function) {
    let dataString = (Buffer.from(req.data.data, 'base64').toString());
    let data = JSON.parse(dataString) as RoomDataPoint;
    if(!data) return;

    let timestamp = new Date(data.timestamp);
    let dateKey = timestamp.getTime().toString()

    const key = datastore.key([kind, dateKey])
    const entity = {
        key: key,
        data: {
          temperature: data ? data.temperature : null,
          humidity: data ? data.humidity : null,
          ambientLight: data ? data.ambientLight : null,
          timestamp: timestamp.toString()
        } as RoomDataPoint
    };
    datastore.save(entity)
        .then()
        .catch((err: Error) => console.error(err));

    callback();
}