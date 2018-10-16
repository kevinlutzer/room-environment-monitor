import * as admin from 'firebase-admin';

export interface RoomEnvironmentMonitorPubsubMessageInterface {
    lux: number;
    co2: number;
    tvoc: number;
    room_temp: number;
    cpu_temp: number;
    pressure: number;
    humidity: number;
    timestamp: string;
}

export class RoomEnvironmentMonitorTelemetry {
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

export interface RoomEnvironmentMonitorLookupApiRequestInteface {
    cursor: string;
    page_size: string;
    device_id: string;
}