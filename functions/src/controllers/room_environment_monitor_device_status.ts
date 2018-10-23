// import * as functions from 'firebase-functions';
import * as admin   from 'firebase-admin';

import {RoomEnvironmentMonitorDeviceStatusModel} from '../config';
import {IOTPubsubMessageInterface} from '../model/iot_pubsub_message.interface';
import {ExtractInterfaceFromPubsubMessage} from '../util/pubsub';
import {RoomEnvironmentMonitorDeviceStatusPubsubMessageInterface, RoomEnvironmentMonitorDeviceStatusInterface, Convert} from '../model/room_environment_monitor_device_status.interface';

const db = admin.firestore();
try {
    db.settings({timestampsInSnapshots: true});
} catch {}

// Handlers
export async function PubsubHandler(message: IOTPubsubMessageInterface) {
    const rawData = ExtractInterfaceFromPubsubMessage(message) as RoomEnvironmentMonitorDeviceStatusPubsubMessageInterface;
    const data = Convert(rawData);

    const deviceId = message.attributes.deviceId;
    const id = deviceId;

    return createRoomEnvironmentMonitorDeviceStatusEntity(id, data);
}

async function createRoomEnvironmentMonitorDeviceStatusEntity(id: string, data: RoomEnvironmentMonitorDeviceStatusInterface) {
    return db
        .collection(RoomEnvironmentMonitorDeviceStatusModel)
        .doc(id)
        .set(data)
        .then(
            resp => console.log("Added new entity with id: ", id, " and resp: ", resp),
            err => console.error("Failed to create entity: ", err)
        );
}