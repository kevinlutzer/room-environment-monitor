import * as admin from 'firebase-admin';

export const TelemetryEventModel = "TelemetryEvent"

export interface Telemetry {
    lux: number;
    co2: number;
    tvoc: number;
    roomTemp: number;
    cpuTemp: number;
    pressure: number;
    humidity: number;
}

export interface TelemetryEvent extends Telemetry {
    timestamp: admin.firestore.Timestamp;
    deviceId: string;
}