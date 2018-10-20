import * as admin   from 'firebase-admin';

import {Request, Response} from 'express';

import {RoomEnvironmentMonitorTelemetry, RoomEnvironmentMonitorPubsubMessageInterface, RoomEnvironmentMonitorListApiRequestInteface, Convert} from '../model/room-environment-telemetry.api.model';
import {IOTPubsubMessageInterface} from '../model/iot-pubsub-message.interface';
import {RoomEnvironmentTelemetryModel} from '../config';
import {ExtractInterfaceFromPubsubMessage} from '../util/pubsub';

const db = admin.firestore();
try {
    db.settings({timestampsInSnapshots: true});
} catch {}

// Handlers
export const PubsubHandler = async (message: IOTPubsubMessageInterface) => {

    const rawData = ExtractInterfaceFromPubsubMessage(message) as RoomEnvironmentMonitorPubsubMessageInterface;

    // Becasue the id is based on the timestamp, if the timestamp is passed null or undefined the same entity will be updated
    const sysDate = new Date(rawData.timestamp || "");
    const deviceId = message.attributes.deviceId;
    const id = deviceId + ":" + sysDate.getTime().toString();
    
    const data = Convert(deviceId, sysDate, rawData)

    return createRoomEnvironmentMonitorTelemetryEntity(id, data);
}

async function createRoomEnvironmentMonitorTelemetryEntity(id: string, data: RoomEnvironmentMonitorTelemetry) {
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
