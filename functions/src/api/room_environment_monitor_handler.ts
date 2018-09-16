import * as functions from 'firebase-functions';
import * as admin   from 'firebase-admin';

import {RoomEnvironmentMonitorTelemetry, RoomEnvironmentMonitorTelemetryApiInterface} from '../model/room-environment-monitor-telemetry.api.model';

admin.initializeApp(functions.config().firebase);

const db = admin.firestore();
db.settings({timestampsInSnapshots: true});

export const RoomEnvironmentMonitorHandler = functions.pubsub.topic('room-monitor-telemetry').onPublish((message) => {
    let rawData: RoomEnvironmentMonitorTelemetryApiInterface;
    try {
        rawData = JSON.parse(message.data ? Buffer.from(message.data, 'base64').toString() : null);
    } catch {
        console.error("Failed to parse the pubsub mesage. The api interface most likely changed");
        return null;
    }

    // Becasue the id is based on the timestamp, if the timestamp is passed null or undefined the same entity will be updated
    const sysDate = new Date(rawData.timestamp || "");
    const id = sysDate.getTime().toString();

    var data = {
        lux: rawData.lux || 0,
        co2: rawData.co2 || 0,
        tvoc: rawData.tvoc || 0,
        roomTemp: rawData.room_temp || 0,
        cpuTemp: rawData.cpu_temp || 0,
        timestamp: admin.firestore.Timestamp.fromDate(sysDate),
    } as RoomEnvironmentMonitorTelemetry

    return db
        .collection('RoomMonitorTeletry')
        .doc(id)
        .set(data)
        .then(
            resp => console.log("Added new entity with id: ", id, " and resp: ", resp),
            err => console.error("Failed to create entity: ", err)
        );
});

(new Date()).toTimeString()