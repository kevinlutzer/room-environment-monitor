import * as functions from 'firebase-functions';

import { RoomEnvironmentMonitorTelemetryInterface, RoomEnvironmentMonitorTelemetryPubsubMessageInterface, 
    ConvertPubsubMessage, TelemetryModel, 
    ConvertQuerySnapshotDocument } from '../models/telemetry.interface';
import { ExtractInterfaceFromPubsubMessage } from './pubsub.util';
import { Response, Request, NextFunction } from 'express';
import { ApiRequest, ApiResponse } from './api.interface';

// Handlers
export async function TelemetryList(req: Request, res: Response, next: NextFunction, db: FirebaseFirestore.Firestore) {
    const r = ApiRequest.fromRequest(req);

    let q = db
        .collection(TelemetryModel)
        .limit(r.pageSize)
        .offset(r.cursor)

    if (r.searchTerm) {
        q = q.where('deviceId', '==', r.searchTerm);
    }    

    return q
        .get()
        .then(results => handleListResponse(r, res, results))
        .catch(next);
}

function handleListResponse(req: ApiRequest, res: Response, s: FirebaseFirestore.QuerySnapshot) {
    const data = (s.docs || []).map(ConvertQuerySnapshotDocument);
    (new ApiResponse<RoomEnvironmentMonitorTelemetryInterface>(data, req)).toReponse(res);
}


export async function TelemetryCreatePubsubHandler(message: functions.pubsub.Message, db: FirebaseFirestore.Firestore) {
    const rawData = ExtractInterfaceFromPubsubMessage(message) as RoomEnvironmentMonitorTelemetryPubsubMessageInterface;

    if (!rawData) {
        console.error("The raw data could not be abstracted fro the pubsub message.");
        return
    }

    // Becasue the id is based on the timestamp, if the timestamp is passed null or undefined the same entity will be updated
    const sysDate = new Date(rawData.timestamp || "");
    const deviceId = message.attributes.deviceId;
    const id = deviceId + ":" + sysDate.getTime().toString();
    
    const data = ConvertPubsubMessage(deviceId, sysDate, rawData)

    return create(id, data, db);
}

async function create(id: string, data: RoomEnvironmentMonitorTelemetryInterface, db: FirebaseFirestore.Firestore) {
    return db
    .collection(TelemetryModel)
    .doc(id)
    .set(data)
    .then(
        resp => console.log("Added new entity with id: ", id, " and resp: ", resp),
        err => console.error("Failed to create entity: ", err)
    );
}


