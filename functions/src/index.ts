import * as functions from 'firebase-functions';
import * as admin   from 'firebase-admin';
import * as express from 'express';
import * as cors from 'cors';
import { RoomEnvironmentMonitorHandler } from './api/room_environment_monitor_handler';

admin.initializeApp(functions.config().firebase);
const db = admin.firestore();
db.settings({timestampsInSnapshots: true});

const handler = new RoomEnvironmentMonitorHandler();

const app = express();
app.use(cors({ origin: true }));

// CRUD interfaces:
app.get('/list', (req, res) => handler.List(req, res, db));

// Pubsub handlers
exports.RoomEnvironmentMonitorPubsubHandler = functions.pubsub.topic('room-monitor-telemetry').onPublish((message) => handler.Pubsub(message, db))
exports.RoomEnvironmentMonitorHandler = functions.https.onRequest(app);