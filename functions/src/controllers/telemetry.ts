import * as functions from 'firebase-functions';

import { RoomEnvironmentMonitorTelemetryInterface, RoomEnvironmentMonitorTelemetryPubsubMessageInterface, 
    ConvertPubsubMessage, TelemetryModel, 
    ConvertQuerySnapshotDocument } from '../models/telemetry.interface';
import { ExtractInterfaceFromPubsubMessage } from './pubsub.util';
import { Response, Request, NextFunction } from 'express';
import { ApiListRequest, ApiListResponse, ApiError } from './api.interface';
import { QuerySnapshot } from '@google-cloud/firestore';

export async function TelemetryGetLatest(req: Request, res: Response, next: NextFunction, db: FirebaseFirestore.Firestore) {
    const deviceId = req.query.deviceId || '';
    return getLatest(deviceId, db)
        .then(result => {
            if (!result.docs || result.docs.length < 1) {
                const msg = `device with id ${deviceId} has no telemtery`
                console.warn(msg);
                (new ApiError(msg, 404)).toApi(res)
                return
            }
            res.json({
                telemetry: ConvertQuerySnapshotDocument(result.docs[0])
            })
        }).catch(err => {
            console.error(err);
            (new ApiError(err, 500)).toApi(res)
        })
}

function getLatest(deviceId: string, db: FirebaseFirestore.Firestore): Promise<QuerySnapshot<FirebaseFirestore.DocumentData>> {
    return db
        .collection(TelemetryModel)
        .limit(1)
        .where('deviceId', '==', deviceId)
        .orderBy('timestamp', 'desc')
        .get()
}

// Handlers
export async function TelemetryList(req: Request, res: Response, next: NextFunction, db: FirebaseFirestore.Firestore) {
    const r = ApiListRequest.fromRequest(req);

    return list(r.cursor, r.pageSize, r.searchTerm, db)
        .then(results => {
            const data = (results.docs || []).map(ConvertQuerySnapshotDocument);
            (new ApiListResponse<RoomEnvironmentMonitorTelemetryInterface>(data, r)).toReponse(res);
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


