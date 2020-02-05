const functions = require('firebase-functions');
const admin = require('firebase-admin');
const express = require('express');

import { TelemetryPubsubHandler, TelemetryEventList, DeviceList } from './api';

admin.initializeApp(functions.config().firebase);

const db = admin.firestore();
db.settings({timestampsInSnapshots: true});

const RoomEnvironmentMonitor = express();

RoomEnvironmentMonitor.get('/api/telemetry-event/list', (req, res, _) => TelemetryEventList(req, res, db))
RoomEnvironmentMonitor.get('/api/device/list', (req, res, _) => DeviceList(req, res, db))


exports.TelemetryEventPubsubHandler = functions.pubsub.topic('room-environment-monitor-telemetry').onPublish(msg => TelemetryPubsubHandler(msg, db))
exports.RoomEnvironmentMonitor = functions.https.onRequest(RoomEnvironmentMonitor);