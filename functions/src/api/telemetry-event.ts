import { Request, Response } from 'express';
import { TelemetryEventModel } from '../models';
import { convertFirestoreDocsToTelemetryEvent } from './api-conversion';
import { DocumentSnapshot } from '@google-cloud/firestore';

export async function TelemetryEventList(req: Request, res: Response, db: FirebaseFirestore.Firestore) {
    const pageSize = parseInt(req.query.page_size, 10) || 100;
    const cursor = parseInt(req.query.cursor, 10) || 0;
    const deviceId = (req.query.device_id as string);
    
    let q: FirebaseFirestore.QuerySnapshot<FirebaseFirestore.DocumentData>
    try {
        q = await list(pageSize, cursor, deviceId, db) 
    } catch (e) {
        console.error(e);
        res.json({error: e});
        res.status(500);
    }
    const docs = q.docs;
    const hasMore = q.size === pageSize;

    const t = convertFirestoreDocsToTelemetryEvent(docs)
    res.json({
        telemetry: t,
        nextCursor: hasMore ? cursor + q.size : 0, 
        hasMore: hasMore,
    })
}

async function list(pageSize: number, cursor: number, deviceId: string, db: FirebaseFirestore.Firestore): Promise<FirebaseFirestore.QuerySnapshot<FirebaseFirestore.DocumentData>> {
    let query = db.collection(TelemetryEventModel)
        .limit(pageSize)
        .offset(cursor)
        
    if (deviceId) {
       query = query.where('deviceId', '==', deviceId)
    }    
       
    return query
        .orderBy('timestamp', 'desc')
        .get()
}