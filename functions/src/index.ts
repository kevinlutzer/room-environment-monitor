const functions = require('firebase-functions');
const admin = require('firebase-admin');
// const express = require('express');

import { TelemetryEventPubsubHandler } from './api/telemetry-event';

admin.initializeApp(functions.config().firebase);

const db = admin.firestore();
db.settings({timestampsInSnapshots: true});

// const RoomEnvironmentMonitor = express();

// RoomEnvironmentMonitor.get('/api/telemetry/get', (req, res, next) => TelemetryGetLatest(req, res, next, db))
// RoomEnvironmentMonitor.get('/api/telemetry/list', (req, res, next) => TelemetryList(req, res, next, db))

exports.TelemetryEventPubsubHandler = functions.pubsub.topic('room-environment-monitor-telemetry').onPublish(msg => TelemetryEventPubsubHandler(msg, db))
// exports.RoomEnvironmentMonitor = functions.https.onRequest(RoomEnvironmentMonitor);