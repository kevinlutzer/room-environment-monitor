import * as admin from 'firebase-admin';
import { Request } from 'express';

export const MODEL = "RoomEnvironmentMonitorStatus";

export interface RoomEnvironmentMonitorStatusPubsubMessageInterface {
    cpu_temp: number;
    timestamp: string;
}

export interface RoomEnvironmentMonitorStatusInterface {
    cpuTemp: number;
    timestamp: admin.firestore.Timestamp;
    deviceId: string;
}

export function Convert(deviceId: string, sysDate: Date, data: RoomEnvironmentMonitorStatusPubsubMessageInterface): RoomEnvironmentMonitorStatusInterface {
    return {
        cpuTemp: data.cpu_temp || 0,
        timestamp: admin.firestore.Timestamp.fromDate(sysDate),
        deviceId: deviceId,
    } as RoomEnvironmentMonitorStatusInterface;
}

export interface RoomEnvironmentMonitorStatusListRequest {
    cursor: number;
    pageSize: number;
} 

export function BuildRoomEnvironmentStatusListRequest(req: Request<any>): RoomEnvironmentMonitorStatusListRequest {
    // return {
    //     cursor: parseInt(req.query.cursor, 10) || 0,
    //     pageSize: parseInt(req.query.pageSize, 10) || 1000
    // }
    return {
        cursor: 0,
        pageSize: 1000
    }
}

export function ConvertQuerySnapshotDocument(snapshot: FirebaseFirestore.QueryDocumentSnapshot): RoomEnvironmentMonitorStatusInterface {
    const data = snapshot.data();
    return {
        cpuTemp: data.cpuTemp,
        timestamp: data.timestamp ? data.timestamp.toDate() : new Date(),
        deviceId: data.deviceId,
    }
}