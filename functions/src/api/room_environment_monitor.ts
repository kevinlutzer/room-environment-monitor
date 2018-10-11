import * as functions from 'firebase-functions';
import * as express from 'express';
import * as cors from 'cors';

import { roomEnvironmentDataList, roomEnvironmentMonitorPubsubHandler } from '../controllers/room_environment_monitor';

const app = express();
app.use(cors({ origin: true }));

// CRUD interfaces:
app.get('/list', roomEnvironmentDataList);

export const RoomEnvironmentMonitorPubsubHandler = functions.pubsub.topic('room-monitor-telemetry').onPublish(roomEnvironmentMonitorPubsubHandler)
export const RoomEnvironmentDataList = functions.https.onRequest(app);