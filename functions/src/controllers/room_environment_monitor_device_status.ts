import * as functions from 'firebase-functions';
import * as admin   from 'firebase-admin';

import {IOTPubsubMessageInterface} from '../model/iot_pubsub_message.interface';
import {RoomEnvironmentMonitorDeviceCPUTempThreshold} from '../config';
import {ExtractInterfaceFromPubsubMessage} from '../util/pubsub';
import {RoomEnvironmentMonitorDeviceStatusPubsubMessageInterface, Convert} from '../model/room_environment_monitor_device_status.interface';
import {SendEmail} from './sendgrid';

// Initialize Firebase App
const db = admin.firestore();
try {
    db.settings({timestampsInSnapshots: true});
} catch {}

// Handlers
export async function PubsubHandler(message: IOTPubsubMessageInterface) {
    const rawData = ExtractInterfaceFromPubsubMessage(message) as RoomEnvironmentMonitorDeviceStatusPubsubMessageInterface;
    const data = Convert(rawData);

    return handleCPUTempOverThresh(data.cpuTemp, message.attributes.deviceId);
}

export async function handleCPUTempOverThresh(cpuTemp: number, deviceId: string): Promise<void> {
    if (cpuTemp >= RoomEnvironmentMonitorDeviceCPUTempThreshold) {
        console.log('Cpu Temp is over threshold');
        return SendEmail(['kevinlutzer9@gmail.com'], 'Room Environment Monitor CPU Temp Is Too High - ' + deviceId, 'The cpu temp is ' + cpuTemp.toString())
            .then(() => console.log('Hello World'));
    } else {
        console.log("Cpu Temp is within normal range");
        return Promise.resolve();
    }
}


// export const EntityCreateHandler = async(snapshot: admin.firestore.DocumentSnapshot, context: any) => {
//     const data = snapshot.data() as RoomEnvironmentMonitorTelemetry;
//     if (data.cpuTemp > RoomEnvironmentTelemetryCPUTempThreshold) {
//         console.warn("Reached cpu threshold");
//         
//     } else {
//         console.log("CPU temp is not above threshold")
//         return Promise.resolve();
//     }
// }

