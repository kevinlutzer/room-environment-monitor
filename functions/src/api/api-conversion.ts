import { Request, Response } from 'express';
import { TelemetryEvent } from '../models';

export function convertFirestoreDocsToTelemetryEvent(docs: FirebaseFirestore.QueryDocumentSnapshot<FirebaseFirestore.DocumentData>[]): TelemetryEvent[] {
    let data: FirebaseFirestore.DocumentData;
    return (docs || []).map((doc) => {
        data = doc.data();
        return {
            lux: data.lux,
            co2: data.co2,
            tvoc: data.tvoc,
            roomTemp: data.roomTemp,
            cpuTemp: data.cpuTemp,
            pressure: data.pressure,
            humidity: data.humidity,
            timestamp: data.timestamp ? data.timestamp.toDate() : new Date(),
            deviceId: data.deviceId
        } as TelemetryEvent;
    })

} 