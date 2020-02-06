import { TelemetryEvent, Device } from '../models';

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
            deviceId: data.deviceId,
            id: doc.id
        } as TelemetryEvent;
    })
} 

export function convertFirestoreDocsToDevice(docs: FirebaseFirestore.QueryDocumentSnapshot<FirebaseFirestore.DocumentData>[]): Device[] {
    let data: FirebaseFirestore.DocumentData;
    return (docs || []).map((doc) => {
        data = doc.data();
        return {
            deviceId: doc.id || '',
            name: data.name || '',
            roomLocation: data.roomLocation || '',
            description: data.description || '',
            created: data.created ? data.created.toDate() : new Date(),
            lastTelemetry: {
                lux: data.lastTelemetry.lux || 0,
                co2: data.lastTelemetry.co2 || 0,
                tvoc: data.lastTelemetry.tvoc || 0,
                roomTemp: data.lastTelemetry.roomTemp || 0,
                cpuTemp: data.lastTelemetry.cpuTemp || 0,
                pressure: data.lastTelemetry.pressure || 0,
                humidity: data.lastTelemetry.humidity || 0,
            },
            lastActivity: data.lastActivity ? data.lastActivity.toDate() : new Date(),
        } as Device;
    })

} 