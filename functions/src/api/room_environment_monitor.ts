import * as functions from 'firebase-functions';
import * as express from 'express';
import * as cors from 'cors';

import { dataList, pubsubHandler } from '../controllers/room_environment_monitor';

const app = express();
app.use(cors({ origin: true }));

// CRUD interfaces:
app.get('/list', dataList);


export const RoomEnvironmentMonitorPubsubHandler = functions.pubsub.topic('room-monitor-telemetry').onPublish(pubsubHandler)
export const RoomEnvironmentDataList = functions.https.onRequest(app);