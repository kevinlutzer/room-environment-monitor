import * as functions from 'firebase-functions';
import { telemetryEventFromPubsubMessage, telemetryPubsubMessageInterface, 
    dataFromPubsubMessage } from './pubsub-conversion';
import { TelemetryEventModel } from '../models';

export async function TelemetryEventPubsubHandler(message: functions.pubsub.Message, db: FirebaseFirestore.Firestore) {
    const rawData = dataFromPubsubMessage(message) as telemetryPubsubMessageInterface;
    if (!rawData) {
        console.error("The raw data could not be abstracted fro the pubsub message.");
        return
    }

    // Because the id is based on the timestamp, if the timestamp is passed null or undefined the same entity will be updated
    const sysDate = new Date(rawData.timestamp || "");
    const deviceId = message.attributes.deviceId;
    
    const te = telemetryEventFromPubsubMessage(deviceId, sysDate, rawData);
    
    return db.collection(TelemetryEventModel).add(te)
}


// Handlers