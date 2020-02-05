import * as admin from 'firebase-admin';
import { Telemetry } from './telemetry-event.interface';

export const DeviceModel = "Device"

export interface Device {
    deviceId: string;
    roomLocation: string;
    description: string;
    created: admin.firestore.Timestamp;
    lastTelemetry: Telemetry;
}