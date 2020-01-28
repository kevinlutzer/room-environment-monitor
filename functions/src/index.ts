const functions = require('firebase-functions');
const admin = require('firebase-admin');
const express = require('express');

import { TelemetryCreatePubsubHandler, TelemetryList,
    StatusUpdatePubsubHandler, StatusList} from './controllers';

export const RoomEnvironmentTelemetryPubsubTopic = 'room-environment-monitor-telemetry';
export const RoomEnvironmentStatusPubsubTopic = 'room-environment-monitor-status';

admin.initializeApp(functions.config().firebase);

const db = admin.firestore();
db.settings({timestampsInSnapshots: true});

const RoomEnvironmentMonitor = express();

RoomEnvironmentMonitor.get('/api/telemetry/list', (req, res, next) => TelemetryList(req, res, next, db))
RoomEnvironmentMonitor.get('/api/status/list', (req, res, next) => StatusList(req, res, next, db))

exports.TelemetryCreatePubsubHandler = functions.pubsub.topic(RoomEnvironmentTelemetryPubsubTopic).onPublish(msg => TelemetryCreatePubsubHandler(msg, db))
exports.StatusUpsertPubsubHandler = functions.pubsub.topic(RoomEnvironmentTelemetryPubsubTopic).onPublish(msg => StatusUpdatePubsubHandler(msg, db))
exports.RoomEnvironmentMonitor = functions.https.onRequest(RoomEnvironmentMonitor);