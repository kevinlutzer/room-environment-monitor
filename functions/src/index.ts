import * as functions from 'firebase-functions';
import * as admin   from 'firebase-admin';
import * as express from 'express';
import * as cors from 'cors';

import { RoomEnvironmentMonitorHandler } from './controllers/room_environment_monitor_handler';

admin.initializeApp(functions.config().firebase);
const db = admin.firestore();
db.settings({timestampsInSnapshots: true});

const roomEnvironmentHandler = new RoomEnvironmentMonitorHandler();

const roomEnvironmentMonitorApp = express();
roomEnvironmentMonitorApp.use(cors({ origin: true }));

// CRUD interfaces:
roomEnvironmentMonitorApp.get('/list', (req, res) => roomEnvironmentHandler.List(req, res, db));

// Pubsub handlers
exports.RoomEnvironmentMonitorPubsubHandler = functions.pubsub.topic('room-monitor-telemetry').onPublish((message) => roomEnvironmentHandler.Pubsub(message, db))
exports.RoomEnvironmentMonitorHandler = functions.https.onRequest(roomEnvironmentMonitorApp);


export {IOTFullfillmentHandler} from './api/iot_fullfillment';