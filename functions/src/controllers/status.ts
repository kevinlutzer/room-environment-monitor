import * as functions from 'firebase-functions';
import { SetOptions } from 'firestore';

import { RoomEnvironmentMonitorStatusInterface, RoomEnvironmentMonitorStatusPubsubMessageInterface,
    Convert, StatusModel, ConvertQuerySnapshotDocument } from '../models/status.interface';
import { ExtractInterfaceFromPubsubMessage } from './pubsub.util';
import { Response, Request, NextFunction } from 'express';
import { ApiResponse, ApiRequest } from './api.interface'; 

// Handlers
export async function StatusUpdatePubsubHandler(message: functions.pubsub.Message, db: FirebaseFirestore.Firestore) {
    const rawData = ExtractInterfaceFromPubsubMessage(message) as RoomEnvironmentMonitorStatusPubsubMessageInterface;

    if (!rawData) {
        console.error("The raw data could not be abstracted fro the pubsub message.");
        return
    }

    // Becasue the id is based on the timestamp, if the timestamp is passed null or undefined the same entity will be updated
    const sysDate = new Date(rawData.timestamp || "");
    const deviceId = message.attributes.deviceId;
    const data = Convert(deviceId, sysDate, rawData)

    return upsert(deviceId, data, db, {} as SetOptions);
}

async function upsert(id: string, data: RoomEnvironmentMonitorStatusInterface, db: FirebaseFirestore.Firestore, options: SetOptions) {
    return db
    .collection(StatusModel)
    .doc(id)
    .set(data, options)
    .catch(err => console.error(err));
}

// Handlers
export async function StatusList(req: Request, res: Response, next: NextFunction, db: FirebaseFirestore.Firestore) {
    const r = ApiRequest.fromRequest(req);

    return db
        .collection(StatusModel)
        .limit(r.pageSize)
        .offset(r.cursor)
        .get()
        .then(results => {
            const data = (results.docs || []).map(ConvertQuerySnapshotDocument);
            (new ApiResponse<RoomEnvironmentMonitorStatusInterface>(data, r)).toReponse(res);
        })
        .catch(err => console.error(err));
}