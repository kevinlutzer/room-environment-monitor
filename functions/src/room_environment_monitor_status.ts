import * as functions from 'firebase-functions';

import {RoomEnvironmentMonitorStatusInterface, RoomEnvironmentMonitorStatusPubsubMessageInterface, Convert, MODEL,
    BuildRoomEnvironmentStatusListRequest, RoomEnvironmentMonitorStatusListRequest, ConvertQuerySnapshotDocument} from './room_environment_monitor_status.interface';
import {ExtractInterfaceFromPubsubMessage} from './pubsub.util';
import { Response, Request, NextFunction } from 'express';

// Handlers
export async function PubsubHandler(message: functions.pubsub.Message, db: FirebaseFirestore.Firestore) {
    const rawData = ExtractInterfaceFromPubsubMessage(message) as RoomEnvironmentMonitorStatusPubsubMessageInterface;

    if (!rawData) {
        console.error("The raw data could not be abstracted fro the pubsub message.");
        return
    }

    // Becasue the id is based on the timestamp, if the timestamp is passed null or undefined the same entity will be updated
    const sysDate = new Date(rawData.timestamp || "");
    const deviceId = message.attributes.deviceId;
    const data = Convert(deviceId, sysDate, rawData)

    return createRoomEnvironmentMonitorStatusEntity(deviceId, data, db);
}

async function createRoomEnvironmentMonitorStatusEntity(id: string, data: RoomEnvironmentMonitorStatusInterface, db: FirebaseFirestore.Firestore) {
    return db
    .collection(MODEL)
    .doc(id)
    .set(data)
    .then(
        resp => console.log("Added new entity with id: ", id, " and resp: ", resp),
        err => console.error("Failed to create entity: ", err)
    );
}

// Handlers
export async function List(req: Request, res: Response, next: NextFunction, db: FirebaseFirestore.Firestore) {
    const r = BuildRoomEnvironmentStatusListRequest(req);
    
res.status

    return db
        .collection(MODEL)
        .limit(r.pageSize)
        .offset(r.cursor)
        .get()
        .then(results => handleListResponse(r, res, results))
        .catch(next);
}

function handleListResponse(r: RoomEnvironmentMonitorStatusListRequest, res: Response, s: FirebaseFirestore.QuerySnapshot) {
    const hasMore = r.pageSize === s.size;
    res.json({
        data: (s.docs || []).map(ConvertQuerySnapshotDocument),
        hasMore: hasMore,
        nextCursor: hasMore ? r.pageSize + r.cursor : 0
    })
}


