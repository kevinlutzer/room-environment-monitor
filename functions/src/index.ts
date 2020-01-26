const functions = require('firebase-functions');
const admin = require('firebase-admin');
const express = require('express');

import {PubsubHandler as TelemetryPubsubHandler, List as TelemetryListHandler} from './room_environment_monitor_telemetry';
import {PubsubHandler as StatusPubsubHandler, List as StatusListHandler} from './room_environment_monitor_status';
import { SetConfig } from './room_environment_monitor_config_webhook';


export const RoomEnvironmentTelemetryPubsubTopic = 'room-environment-monitor-telemetry';
export const RoomEnvironmentStatusPubsubTopic = 'room-environment-monitor-status';

admin.initializeApp(functions.config().firebase);

const db = admin.firestore();
db.settings({timestampsInSnapshots: true});

const RoomEnvironmentTelemetryExpressApp = express();
const RoomEnvironmentStatusExpressApp = express();

RoomEnvironmentTelemetryExpressApp.get('/api/v1/list', (req, res, next) => TelemetryListHandler(req, res, next, db))
RoomEnvironmentTelemetryExpressApp.post('/api/v1/set-config', (req, res, next) => SetConfig(req, res, next, db))

RoomEnvironmentStatusExpressApp.get('/api/v1/list', (req, res, next) => StatusListHandler(req, res, next, db))

exports.RoomEnvironmentTelemetryPubsubHandler = functions.pubsub.topic(RoomEnvironmentTelemetryPubsubTopic).onPublish(msg => TelemetryPubsubHandler(msg, db))
exports.RoomEnvironmentStatusPubsubHandler = functions.pubsub.topic(RoomEnvironmentTelemetryPubsubTopic).onPublish(msg => StatusPubsubHandler(msg, db))
exports.RoomEnvironmentTelemetryHandler = functions.https.onRequest(RoomEnvironmentTelemetryExpressApp);
exports.RoomEnvironmentStatusHandler = functions.https.onRequest(RoomEnvironmentStatusExpressApp);