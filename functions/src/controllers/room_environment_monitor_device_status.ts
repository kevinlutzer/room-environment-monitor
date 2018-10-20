import * as functions from 'firebase-functions';
import * as admin   from 'firebase-admin';

import {IOTPubsubMessageInterface} from '../model/iot-pubsub-message.interface';
import {RoomEnvironmentMonitorDeviceCPUTempThreshold} from '../config';
import {ExtractInterfaceFromPubsubMessage} from '../util/pubsub';
import {RoomEnvironmentMonitorDeviceStatusPubsubMessageInterface, Convert } from '../model/room_environment_monitor_device_status';

// Initialize Firebase App
admin.initializeApp(functions.config().firebase);
const db = admin.firestore();
try {
    db.settings({timestampsInSnapshots: true});
} catch {}

// Handlers
export const PubsubHandler = async (message: IOTPubsubMessageInterface) => {
    const rawData = ExtractInterfaceFromPubsubMessage(message) as RoomEnvironmentMonitorDeviceStatusPubsubMessageInterface;
    const data = Convert(rawData);
    
    return handleCPUTempOverThresh(data.cpuTemp)
}

async function handleCPUTempOverThresh(cpuTemp: number) {
    if (cpuTemp >= RoomEnvironmentMonitorDeviceCPUTempThreshold) {
        console.log("Cpu Temp is over threshold")
    } else {
        console.log("Cpu Temp is within normal range")
    }
}

// export const EntityCreateHandler = async(snapshot: admin.firestore.DocumentSnapshot, context: any) => {
//     const data = snapshot.data() as RoomEnvironmentMonitorTelemetry;
//     if (data.cpuTemp > RoomEnvironmentTelemetryCPUTempThreshold) {
//         console.warn("Reached cpu threshold");
//         return SendEmail(['kevinlutzer9@gmail.com'], 'Room Environment Monitor Temp Is Too High', 'The cpu temp is ' + data.cpuTemp.toString())
//     } else {
//         console.log("CPU temp is not above threshold")
//         return Promise.resolve();
//     }
// }

