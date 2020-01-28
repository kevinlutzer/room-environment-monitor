import * as functions from 'firebase-functions';

import { RoomEnvironmentMonitorStatusInterface, RoomEnvironmentMonitorStatusPubsubMessageInterface,
    Convert, StatusModel, ConvertQuerySnapshotDocument } from '../models/status.interface';
import { ExtractInterfaceFromPubsubMessage } from './pubsub.util';
import { Response, Request, NextFunction } from 'express';
import { ApiListResponse, ApiListRequest } from './api.interface'; 
import { SetOptions, QuerySnapshot } from '@google-cloud/firestore';

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

    return upsert(deviceId, data, db);
}

async function upsert(id: string, data: RoomEnvironmentMonitorStatusInterface, db: FirebaseFirestore.Firestore) {
    return db
    .collection(StatusModel)
    .doc(id)
    .set(data, {mergeFields: ["cpuTemp", "timestamp"]} as SetOptions)
    .catch(err => console.error(err));
}

// Handlers
export async function StatusList(req: Request, res: Response, next: NextFunction, db: FirebaseFirestore.Firestore) {
    const r = ApiListRequest.fromRequest(req);

    return list(r.cursor, r.pageSize, db)
        .then(results => {
            const data = (results.docs || []).map(ConvertQuerySnapshotDocument);
            (new ApiListResponse<RoomEnvironmentMonitorStatusInterface>(data, r)).toReponse(res);
        })
        .catch(err => console.error(err));
}

function list(cursor: number, pageSize: number, db: FirebaseFirestore.Firestore): Promise<QuerySnapshot<FirebaseFirestore.DocumentData>> {
    return db
    .collection(StatusModel)
    .limit(pageSize)
    .offset(cursor)
    .get()
}