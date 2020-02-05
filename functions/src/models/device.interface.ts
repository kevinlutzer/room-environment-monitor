import * as admin from 'firebase-admin';
import { Telemetry } from './telemetry-event.interface';

export const DeviceModel = "Device"

export interface Device {
    deviceId: string;
    name: string;
    roomLocation: string;
    description: string;
    created: admin.firestore.Timestamp;
    lastTelemetry: Telemetry;
    lastActivity: admin.firestore.Timestamp;
}