import * as admin from 'firebase-admin';
import { Request } from 'express';

export const TelemetryModel = 'RoomEnvironmentTelemetry';

export interface RoomEnvironmentMonitorTelemetryListRequestInterface {
    deviceId: string;
    cursor: number;
    pageSize: number;
}

export interface RoomEnvironmentMonitorTelemetryPubsubMessageInterface {
    lux: number;
    co2: number;
    tvoc: number;
    room_temp: number;
    cpu_temp: number;
    pressure: number;
    humidity: number;
    timestamp: string;
}

export interface RoomEnvironmentMonitorTelemetryInterface {
    lux: number;
    co2: number;
    tvoc: number;
    roomTemp: number;
    cpuTemp: number;
    pressure: number;
    humidity: number;
    timestamp: admin.firestore.Timestamp;
    deviceId: string;
}

export function ConvertPubsubMessage(deviceId: string, sysDate: Date, data: RoomEnvironmentMonitorTelemetryPubsubMessageInterface): RoomEnvironmentMonitorTelemetryInterface {
    return {
        lux: data.lux || 0,
        co2: data.co2 || 0,
        tvoc: data.tvoc || 0,
        roomTemp: data.room_temp || 0,
        cpuTemp: data.cpu_temp || 0,
        pressure: data.pressure || 0, 
        humidity: data.humidity || 0,
        timestamp: admin.firestore.Timestamp.fromDate(sysDate),
        deviceId: deviceId,
    } as RoomEnvironmentMonitorTelemetryInterface
}

export function BuildRoomEnvironmentTelemetryListRequest(req: Request): RoomEnvironmentMonitorTelemetryListRequestInterface {
    return {
        deviceId: '',
        cursor: 0,
        pageSize: 1000
    }
}

export function ConvertQuerySnapshotDocument(snapshot: FirebaseFirestore.QueryDocumentSnapshot): RoomEnvironmentMonitorTelemetryInterface {
    const data = snapshot.data();
    return {
        lux: data.lux,
        co2: data.co2,
        tvoc: data.tvoc,
        roomTemp: data.roomTemp,
        cpuTemp: data.cpuTemp,
        pressure: data.pressure,
        humidity: data.humidity,
        timestamp: data.timestamp ? data.timestamp.toDate() : new Date(),
        deviceId: data.deviceId,
    }
}
