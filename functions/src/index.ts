import * as functions from 'firebase-functions';
import * as admin   from 'firebase-admin';
import * as express from 'express';
import * as cors from 'cors';

import { RoomEnvironmentMonitorHandler } from './api/room_environment_monitor_handler';
import { IOTFullfillmentHandler } from './api/iot_fullfillment';

admin.initializeApp(functions.config().firebase);
const db = admin.firestore();
db.settings({timestampsInSnapshots: true});

const roomEnvironmentHandler = new RoomEnvironmentMonitorHandler();
const iotFullfillmentHandler = new IOTFullfillmentHandler();

const roomEnvironmentMonitorApp = express();
roomEnvironmentMonitorApp.use(cors({ origin: true }));

const iotFullFillmentApp = express();
iotFullFillmentApp.use(cors({ origin: true }));

// CRUD interfaces:
roomEnvironmentMonitorApp.get('/list', (req, res) => roomEnvironmentHandler.List(req, res, db));
iotFullFillmentApp.post('/', iotFullfillmentHandler.FullFillment);

// Pubsub handlers
exports.RoomEnvironmentMonitorPubsubHandler = functions.pubsub.topic('room-monitor-telemetry').onPublish((message) => roomEnvironmentHandler.Pubsub(message, db))
exports.RoomEnvironmentMonitorHandler = functions.https.onRequest(roomEnvironmentMonitorApp);

exports.IOTFullfillmentHandler = functions.https.onRequest(iotFullFillmentApp)