"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const functions = require("firebase-functions");
const admin = require("firebase-admin");
const room_environment_monitor_telemetry_api_model_1 = require("../model/room-environment-monitor-telemetry.api.model");
// // Start writing Firebase Functions
// // https://firebase.google.com/docs/functions/typescript
//
admin.initializeApp(functions.config().firebase);
exports.RoomEnvironmentMonitorHandler = functions.pubsub.topic('room-monitor-telemetry').onPublish((message) => {
    // Decode the PubSub Message body.
    const rawData = JSON.parse(message.data ? Buffer.from(message.data, 'base64').toString() : null);
    const data = room_environment_monitor_telemetry_api_model_1.RoomEnvironmentMonitorTelemetry.fromApi(rawData);
    console.log(data);
});
exports.helloWorld = functions.https.onRequest((request, response) => {
    response.send("Hello from Firebase!");
});
//# sourceMappingURL=room_environment_monitor_handler.js.map