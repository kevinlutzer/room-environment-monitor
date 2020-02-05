import * as functions from 'firebase-functions';
import { Request, Response } from 'express';
import { telemetryEventFromPubsubMessage, telemetryPubsubMessageInterface, 
    dataFromPubsubMessage, deviceFromPubsubMessage } from './pubsub-conversion';
import { TelemetryEventModel, DeviceModel } from '../models';
import { convertFirestoreDocsToTelemetryEvent } from './api-conversion';
import { SetOptions } from '@google-cloud/firestore';

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
    const d = deviceFromPubsubMessage(deviceId, rawData);

    const batch = db.batch();
    batch.create(db.collection(TelemetryEventModel).doc(), te)
    batch.set(db.collection(DeviceModel).doc(deviceId), d, {mergeFields: ["deviceId", "lastTelemetry"]} as SetOptions)
        
    try {
        await batch.commit()
    } catch (e) {
        console.error(e);
    }
}


export async function TelemetryEventList(req: Request, res: Response, db: FirebaseFirestore.Firestore) {
    const pageSize = parseInt(req.query.page_size, 10) || 100;
    const cursor = parseInt(req.query.cursor, 10) || 0;
    const deviceId = (req.query.device_id as string);
    
    let q: FirebaseFirestore.QuerySnapshot<FirebaseFirestore.DocumentData>
    try {
        q = await list(pageSize, cursor, deviceId, db) 
    } catch (e) {
        console.error(e)
        res.json({
            error: e
        })
        res.status(500)
    }
    
    const t = convertFirestoreDocsToTelemetryEvent(q.docs)
    res.json({
        telemetry: t
    })
}

async function list(pageSize: number, cursor: number, deviceId: string, db: FirebaseFirestore.Firestore): Promise<FirebaseFirestore.QuerySnapshot<FirebaseFirestore.DocumentData>> {
    return db.collection(TelemetryEventModel)
        .limit(pageSize)
        .offset(cursor)
        .where('deviceId', '==', deviceId)
        .orderBy('timestamp', 'desc')
        .get()
}