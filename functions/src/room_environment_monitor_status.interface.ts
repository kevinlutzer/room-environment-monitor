import * as admin from 'firebase-admin';

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
