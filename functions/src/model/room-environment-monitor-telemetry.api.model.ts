import * as admin from 'firebase-admin';

export interface RoomEnvironmentMonitorTelemetryApiInterface {
    lux: number;
    co2: number;
    tvoc: number;
    room_temp: number;
    cpu_temp: number;
    timestamp: string;
}

export class RoomEnvironmentMonitorTelemetry {

    lux: number;
    co2: number;
    tvoc: number;
    roomTemp: number;
    cpuTemp: number;
    timestamp: admin.firestore.Timestamp;

}
