import * as functions from 'firebase-functions';
import * as express from 'express';
import * as cors from 'cors';

import { DataList, PubsubHandler } from '../controllers/room_environment_telemetry';
import { RoomEnvironmentTelemetryModel, RoomEnvironmentTelemetryPubsubTopic} from '../config';

const app = express();
app.use(cors({ origin: true }));

// CRUD interfaces:
app.get('/list', DataList);

export const RoomEnvironmentTelemetry = functions.https.onRequest(app);
export const RoomEnvironmentTelemetryPubsubHandler = functions.pubsub.topic(RoomEnvironmentTelemetryPubsubTopic).onPublish(PubsubHandler)
// export const RoomEnvironmentTelemetryOnCreateHandler = functions.firestore.document(RoomEnvironmentTelemetryModel + '/{modelId}').onCreate(EntityCreateHandler)
