import { Request, Response } from 'express';
import { DeviceModel } from '../models';
import { convertFirestoreDocsToDevice } from './api-conversion';

export async function DeviceList(req: Request, res: Response, db: FirebaseFirestore.Firestore) {
    const pageSize = parseInt(req.query.page_size, 10) || 100;
    const cursor = parseInt(req.query.cursor, 10) || 0;
    
    let q: FirebaseFirestore.QuerySnapshot<FirebaseFirestore.DocumentData>
    try {
        q = await list(pageSize, cursor, db) 
    } catch (e) {
        console.error(e)
        res.json({
            error: e
        })
        res.status(500)
    }

    res.json({
        devices: convertFirestoreDocsToDevice(q.docs)
    })
}

async function list(pageSize: number, cursor: number, db: FirebaseFirestore.Firestore): Promise<FirebaseFirestore.QuerySnapshot<FirebaseFirestore.DocumentData>> {
    return db.collection(DeviceModel)
        .limit(pageSize)
        .offset(cursor)
        .get()
}