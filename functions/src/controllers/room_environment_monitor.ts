import * as functions from 'firebase-functions';
import * as admin   from 'firebase-admin';

import {Request, Response} from 'express';

import {RoomEnvironmentMonitorTelemetry, RoomEnvironmentMonitorPubsubMessageInterface, RoomEnvironmentMonitorLookupApiRequestInteface} from '../model/room-environment-telemetry.api.model';
import {IOTPubsubMessageInterface} from '../model/iot-pubsub-message.interface';

admin.initializeApp(functions.config().firebase);
const db = admin.firestore();
db.settings({timestampsInSnapshots: true});

const model = "RoomMonitorTelemetry";

    // Handlers
export const roomEnvironmentMonitorPubsubHandler = async (message: IOTPubsubMessageInterface) => {
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
        const id = sysDate.getTime().toString();

        const data = {
            lux: rawData.lux || 0,
            co2: rawData.co2 || 0,
            tvoc: rawData.tvoc || 0,
            roomTemp: rawData.room_temp || 0,
            cpuTemp: rawData.cpu_temp || 0,
            pressure: rawData.pressure || 0, 
            humidity: rawData.humidity || 0,
            timestamp: admin.firestore.Timestamp.fromDate(sysDate),
            deviceId: message.attributes.deviceId,
        } as RoomEnvironmentMonitorTelemetry

        return db
        .collection(model)
        .doc(id)
        .set(data)
        .then(
            resp => console.log("Added new entity with id: ", id, " and resp: ", resp),
            err => console.error("Failed to create entity: ", err)
        );
    } catch (err) {
        console.error(err);
        return null;
    }
}

export const roomEnvironmentDataList = async(req: Request, res: Response) => {

    const params = req.query as RoomEnvironmentMonitorLookupApiRequestInteface;
    const cursor = params.cursor ? parseInt(params.cursor, 10): 0;
    const pageSize = params.page_size ? parseInt(params.page_size, 10): 1000;

    const results = await db
        .collection(model)
        .offset(cursor)
        .limit(pageSize)
        .get()

    const telemetry = results.docs && results.docs.length ? results.docs.map((v) => (v.data())) : [];
    res.json({
        telemetry: telemetry})
    return
}
