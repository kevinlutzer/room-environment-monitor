import * as functions from 'firebase-functions';
import * as admin   from 'firebase-admin';

import {RoomEnvironmentMonitorTelemetry, RoomEnvironmentMonitorTelemetryApiInterface} from '../model/room-environment-monitor-telemetry.api.model';

// // Start writing Firebase Functions
// // https://firebase.google.com/docs/functions/typescript
//

admin.initializeApp(functions.config().firebase);

export const RoomEnvironmentMonitorHandler = functions.pubsub.topic('room-monitor-telemetry').onPublish((message) => {
    // Decode the PubSub Message body.
    const rawData = JSON.parse(message.data ? Buffer.from(message.data, 'base64').toString() : null);
    const data = RoomEnvironmentMonitorTelemetry.fromApi(rawData);
    console.log(data);
});


export const helloWorld = functions.https.onRequest((request, response) => {
    response.send("Hello from Firebase!");
});