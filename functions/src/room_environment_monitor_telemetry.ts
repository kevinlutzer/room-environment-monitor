import * as functions from 'firebase-functions';

import {RoomEnvironmentMonitorTelemetryInterface, RoomEnvironmentMonitorTelemetryPubsubMessageInterface, Convert, MODEL} from './room_environment_monitor_telemetry.interface';
import {ExtractInterfaceFromPubsubMessage} from './pubsub.util';

// Handlers
export async function PubsubHandler(message: functions.pubsub.Message, db: FirebaseFirestore.Firestore) {
    const rawData = ExtractInterfaceFromPubsubMessage(message) as RoomEnvironmentMonitorTelemetryPubsubMessageInterface;

    if (!rawData) {
        console.error("The raw data could not be abstracted fro the pubsub message.");
        return
    }

    console.log(rawData);

    // Becasue the id is based on the timestamp, if the timestamp is passed null or undefined the same entity will be updated
    const sysDate = new Date(rawData.timestamp || "");
    const deviceId = message.attributes.deviceId;
    const id = deviceId + ":" + sysDate.getTime().toString();
    
    const data = Convert(deviceId, sysDate, rawData)

    return createRoomEnvironmentMonitorTelemetryEntity(id, data, db);
}

async function createRoomEnvironmentMonitorTelemetryEntity(id: string, data: RoomEnvironmentMonitorTelemetryInterface, db: FirebaseFirestore.Firestore) {
    return db
    .collection(MODEL)
    .doc(id)
    .set(data)
    .then(
        resp => console.log("Added new entity with id: ", id, " and resp: ", resp),
        err => console.error("Failed to create entity: ", err)
    );
}

