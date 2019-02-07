import * as functions from 'firebase-functions';
import * as admin   from 'firebase-admin';

import {PubsubHandler} from './room_environment_telemetry';

export const RoomEnvironmentTelemetryPubsubTopic = 'room-environment-monitor-telemetry';

// import {Create} from './api/visitor';

admin.initializeApp(functions.config().firebase);

const db = admin.firestore();
db.settings({timestampsInSnapshots: true});

export const RoomEnvironmentTelemetryPubsubHandler = functions.pubsub.topic(RoomEnvironmentTelemetryPubsubTopic).onPublish(msg => PubsubHandler(msg, db))
