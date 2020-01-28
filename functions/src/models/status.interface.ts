import * as admin from 'firebase-admin';

export const StatusModel = "RoomEnvironmentMonitorStatus";

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

export function ConvertQuerySnapshotDocument(snapshot: FirebaseFirestore.QueryDocumentSnapshot): RoomEnvironmentMonitorStatusInterface {
    const data = snapshot.data();
    return {
        cpuTemp: data.cpuTemp,
        timestamp: data.timestamp ? data.timestamp.toDate() : new Date(),
        deviceId: data.deviceId,
    }
}