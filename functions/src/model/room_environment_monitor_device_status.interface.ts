import * as admin from 'firebase-admin';

export enum Status {
    Unactive = "Unactive",
}

export interface RoomEnvironmentMonitorDeviceStatusPubsubMessageInterface {
    cpu_temp: number;
}

export interface RoomEnvironmentMonitorDeviceStatusInterface {
    cpuTemp: number;
    updated: admin.firestore.Timestamp;
}

export function Convert(data: RoomEnvironmentMonitorDeviceStatusPubsubMessageInterface): RoomEnvironmentMonitorDeviceStatusInterface
{
    return {
        cpuTemp: data.cpu_temp || 0,
        updated: admin.firestore.Timestamp.now(),
    }
}