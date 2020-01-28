import * as functions from 'firebase-functions';

import { RoomEnvironmentMonitorTelemetryInterface, RoomEnvironmentMonitorTelemetryPubsubMessageInterface, 
    ConvertPubsubMessage, TelemetryModel, 
    ConvertQuerySnapshotDocument } from '../models/telemetry.interface';
import { ExtractInterfaceFromPubsubMessage } from './pubsub.util';
import { Response, Request, NextFunction } from 'express';
import { ApiRequest, ApiResponse } from './api.interface';
import { QuerySnapshot } from '@google-cloud/firestore';

// Handlers
export async function TelemetryList(req: Request, res: Response, next: NextFunction, db: FirebaseFirestore.Firestore) {
    const r = ApiRequest.fromRequest(req);

    return list(r.cursor, r.pageSize, r.searchTerm, db)
        .then(results => {
            const data = (results.docs || []).map(ConvertQuerySnapshotDocument);
            (new ApiResponse<RoomEnvironmentMonitorTelemetryInterface>(data, r)).toReponse(res);
        })
        .catch(err => console.error(err));
}

function list(cursor: number, pageSize: number, searchTerm: string, db: FirebaseFirestore.Firestore): Promise<QuerySnapshot<FirebaseFirestore.DocumentData>> {
    let q = db
        .collection(TelemetryModel)
        .limit(pageSize)
        .offset(cursor)

    if (searchTerm) {
        q = q.where('deviceId', '==', searchTerm);
    }    

    return q
        .get()
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


