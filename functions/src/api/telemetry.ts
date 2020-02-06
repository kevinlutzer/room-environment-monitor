import * as functions from 'firebase-functions';
import { telemetryEventFromPubsubMessage, telemetryPubsubMessageInterface, 
    dataFromPubsubMessage, deviceFromPubsubMessage } from './pubsub-conversion';
import { TelemetryEventModel, DeviceModel } from '../models';
import { SetOptions } from '@google-cloud/firestore';

export async function TelemetryPubsubHandler(message: functions.pubsub.Message, db: FirebaseFirestore.Firestore) {
    const rawData = dataFromPubsubMessage(message) as telemetryPubsubMessageInterface;
    if (!rawData) {
        console.error("The raw data could not be abstracted fro the pubsub message.");
        return
    }

    // Because the id is based on the timestamp, if the timestamp is passed null or undefined the same entity will be updated
    const sysDate = new Date(rawData.timestamp || "");
    const deviceId = message.attributes.deviceId;

    const te = telemetryEventFromPubsubMessage(deviceId, sysDate, rawData);
    const d = deviceFromPubsubMessage(deviceId, sysDate, rawData);

    const batch = db.batch();
    batch.create(db.collection(TelemetryEventModel).doc(), te)
    batch.set(db.collection(DeviceModel).doc(deviceId), d, {mergeFields: ["deviceId", "lastTelemetry", "lastActivity"]} as SetOptions)
        
    try {
        await batch.commit()
    } catch (e) {
        console.error(e);
    }
}