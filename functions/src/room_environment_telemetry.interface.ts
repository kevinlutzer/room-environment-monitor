import * as admin from 'firebase-admin';

export const MODEL = "RoomEnvironmentMonitorTelemetry";

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

export function Convert(deviceId: string, sysDate: Date, data: RoomEnvironmentMonitorPubsubMessageInterface): RoomEnvironmentMonitorTelemetryInterface {
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

export interface RoomEnvironmentMonitorListApiRequestInteface {
    cursor?: string;
    page_size?: string;
    device_id?: string;
}