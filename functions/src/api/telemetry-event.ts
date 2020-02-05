import { Request, Response } from 'express';
import { TelemetryEventModel } from '../models';
import { convertFirestoreDocsToTelemetryEvent } from './api-conversion';

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