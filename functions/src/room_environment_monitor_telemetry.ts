import * as functions from 'firebase-functions';

import {RoomEnvironmentMonitorTelemetryInterface, RoomEnvironmentMonitorTelemetryPubsubMessageInterface, 
    ConvertPubsubMessage, BuildRoomEnvironmentTelemetryListRequest, MODEL, ConvertQuerySnapshotDocument} from './room_environment_monitor_telemetry.interface';
import {ExtractInterfaceFromPubsubMessage} from './pubsub.util';
import { Response, Request, NextFunction } from 'express';

// Handlers
export async function List(req: Request, res: Response, next: NextFunction, db: FirebaseFirestore.Firestore) {
    const r = BuildRoomEnvironmentTelemetryListRequest(req);
    r.deviceId
    return db
        .collection(MODEL)
        .where('deviceId', '==', r.deviceId)
        .limit(r.pageSize)
        .offset(r.cursor)
        .get()
        .then(results => handleListResponse(res, results))
        .catch(next);
}

function handleListResponse(res: Response, s: FirebaseFirestore.QuerySnapshot) {
    const telemetry = (s.docs || []).map(ConvertQuerySnapshotDocument)
    res.json(telemetry)
}

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
    
    const data = ConvertPubsubMessage(deviceId, sysDate, rawData)

    return create(id, data, db);
}

async function create(id: string, data: RoomEnvironmentMonitorTelemetryInterface, db: FirebaseFirestore.Firestore) {
    return db
    .collection(MODEL)
    .doc(id)
    .set(data)
    .then(
        resp => console.log("Added new entity with id: ", id, " and resp: ", resp),
        err => console.error("Failed to create entity: ", err)
    );
}


