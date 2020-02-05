import * as functions from 'firebase-functions';
import * as admin from 'firebase-admin';
import { TelemetryEvent } from '../models';

export function dataFromPubsubMessage(message: functions.pubsub.Message): any {
    let rawData: any;
    try {
        rawData = JSON.parse(message.data ? Buffer.from(message.data, 'base64').toString() : null);
    } catch {
        console.error("Failed to parse the pubsub mesage. The api interface most likely changed");
        return null;
    }
    console.log("message: ", message, ", rawData: ", rawData);
    return rawData;
}

export interface telemetryPubsubMessageInterface {
    lux: number;
    co2: number;
    tvoc: number;
    room_temp: number;
    cpu_temp: number;
    pressure: number;
    humidity: number;
    timestamp: string;
}

export function telemetryEventFromPubsubMessage(deviceId: string, sysDate: Date, data: telemetryPubsubMessageInterface): TelemetryEvent {
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
    } as TelemetryEvent;
}
