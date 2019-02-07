import * as functions from 'firebase-functions';
import * as admin   from 'firebase-admin';
import * as express from 'express';
import * as cors from 'cors';

import {DataList, PubsubHandler} from './controllers/room_environment_telemetry';
import {RoomEnvironmentTelemetryPubsubTopic} from './config';


// import {Create} from './api/visitor';

admin.initializeApp(functions.config().firebase);

const db = admin.firestore();
db.settings({timestampsInSnapshots: true});

const app = express();
app.use(cors({ origin: true }));

// CRUD interfaces:
app.get('/list', (req, res) => DataList(req, res, db));

export const RoomEnvironmentTelemetry = functions.https.onRequest(app);
export const RoomEnvironmentTelemetryPubsubHandler = functions.pubsub.topic(RoomEnvironmentTelemetryPubsubTopic).onPublish(msg => PubsubHandler(msg, db))
