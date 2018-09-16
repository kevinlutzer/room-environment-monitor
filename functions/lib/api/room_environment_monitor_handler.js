"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const functions = require("firebase-functions");
const admin = require("firebase-admin");
admin.initializeApp(functions.config().firebase);
const db = admin.firestore();
db.settings({ timestampsInSnapshots: true });
exports.RoomEnvironmentMonitorHandler = functions.pubsub.topic('room-monitor-telemetry').onPublish((message) => {
    let rawData;
    try {
        rawData = JSON.parse(message.data ? Buffer.from(message.data, 'base64').toString() : null);
    }
    catch (_a) {
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
    };
    return db
        .collection('RoomMonitorTeletry')
        .doc(id)
        .set(data)
        .then(resp => console.log("Added new entity with id: ", id, " and resp: ", resp), err => console.error("Failed to create entity: ", err));
});
(new Date()).toTimeString();
//# sourceMappingURL=room_environment_monitor_handler.js.map