import * as functions from 'firebase-functions';
import * as express from 'express';
import * as cors from 'cors';

import { DataList, PubsubHandler, EntityCreateHandler } from '../controllers/room_environment_monitor';
import { RoomEnvironmentTelemetryModel, RoomEnvironmentTelemetryPubsubTopic} from '../config';

const app = express();
app.use(cors({ origin: true }));

// CRUD interfaces:
app.get('/list', DataList);

export const RoomEnvironmentData = functions.https.onRequest(app);

export const RoomEnvironmentMonitorPubsubHandler = functions.pubsub.topic(RoomEnvironmentTelemetryPubsubTopic).onPublish(PubsubHandler)
export const RoomEnvironmentDataOnCreateHandler = functions.firestore.document(RoomEnvironmentTelemetryModel + '/{modelId}').onCreate(EntityCreateHandler)
