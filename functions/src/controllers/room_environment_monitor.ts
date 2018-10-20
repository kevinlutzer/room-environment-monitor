import * as functions from 'firebase-functions';
import * as admin   from 'firebase-admin';

import {Request, Response} from 'express';

import {RoomEnvironmentMonitorTelemetry, RoomEnvironmentMonitorPubsubMessageInterface, RoomEnvironmentMonitorListApiRequestInteface} from '../model/room-environment-telemetry.api.model';
import {IOTPubsubMessageInterface} from '../model/iot-pubsub-message.interface';
import {RoomEnvironmentTelemetryModel, RoomEnvironmentTelemetryCPUTempThreshold} from '../config';

admin.initializeApp(functions.config().firebase);
const db = admin.firestore();
db.settings({timestampsInSnapshots: true});

// Handlers
export const PubsubHandler = async (message: IOTPubsubMessageInterface) => {
    // Wrap whole thing in try catch. If there is an error for some pubsub message, the message will not be acked off the pubsub queue
    try {
        let rawData: RoomEnvironmentMonitorPubsubMessageInterface;
        try {
            rawData = JSON.parse(message.data ? Buffer.from(message.data, 'base64').toString() : null);
        } catch {
            console.error("Failed to parse the pubsub mesage. The api interface most likely changed");
            return null;
        }
        console.log("message: ", message, ", rawData: ", rawData);

        // Becasue the id is based on the timestamp, if the timestamp is passed null or undefined the same entity will be updated
        const sysDate = new Date(rawData.timestamp || "");
        const deviceId = message.attributes.deviceId;
        const id = deviceId + ":" + sysDate.getTime().toString();
        
        const data = {
            lux: rawData.lux || 0,
            co2: rawData.co2 || 0,
            tvoc: rawData.tvoc || 0,
            roomTemp: rawData.room_temp || 0,
            cpuTemp: rawData.cpu_temp || 0,
            pressure: rawData.pressure || 0, 
            humidity: rawData.humidity || 0,
            timestamp: admin.firestore.Timestamp.fromDate(sysDate),
            deviceId: deviceId,
        } as RoomEnvironmentMonitorTelemetry

        return createRoomEnvironmentMonitorTelemetry(id, data);

    } catch (err) {
        console.error(err);
        return null;
    }
}

async function createRoomEnvironmentMonitorTelemetry(id: string, data: RoomEnvironmentMonitorTelemetry) {
    return db
    .collection(RoomEnvironmentTelemetryModel)
    .doc(id)
    .set(data)
    .then(
        resp => console.log("Added new entity with id: ", id, " and resp: ", resp),
        err => console.error("Failed to create entity: ", err)
    );
}

export const DataList = async(req: Request, res: Response) => {

    const params = req.query as RoomEnvironmentMonitorListApiRequestInteface;
    const cursor = params.cursor ? parseInt(params.cursor, 10): 0;
    const pageSize = params.page_size ? parseInt(params.page_size, 10): 1000;
    const deviceId = params.device_id;

    const base = db
        .collection(RoomEnvironmentTelemetryModel)
        .offset(cursor)
        .limit(pageSize)

    if (deviceId && deviceId !== "") {
        base.where("deviceId", "==", deviceId)
    }

    const results = await base.get()

    const telemetry = results.docs && results.docs.length ? results.docs.map((v) => (v.data())) : [];

    res.json({
        telemetry: telemetry
    })

}

export const EntityCreateHandler = (snapshot: admin.firestore.DocumentSnapshot, context: any): Promise<void> => {
    const data = snapshot.data() as RoomEnvironmentMonitorTelemetry;
    if (data.cpuTemp > RoomEnvironmentTelemetryCPUTempThreshold) {
        console.warn("Reached cpu threshold");
        return null;
    } else {
        console.log("CPU temp is not above threshold")
        return null;
    }
}

