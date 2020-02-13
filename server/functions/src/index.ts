const express = require('express');
const cors = require('cors');

import { config as functionsConfig, pubsub, https} from 'firebase-functions';
import { initializeApp, firestore } from 'firebase-admin';

import { TelemetryPubsubHandler, TelemetryEventList, DeviceList } from './api';

initializeApp(functionsConfig().firebase);

const db = firestore();
db.settings({timestampsInSnapshots: true});

const RoomEnvironmentMonitor = express();

RoomEnvironmentMonitor.use(cors());

RoomEnvironmentMonitor.get('/api/telemetry-event/list', (req, res, _) => TelemetryEventList(req, res, db))
RoomEnvironmentMonitor.get('/api/device/list', (req, res, _) => DeviceList(req, res, db))

exports.TelemetryEventPubsubHandler = pubsub.topic('room-environment-monitor-telemetry').onPublish(msg => TelemetryPubsubHandler(msg, db))
exports.RoomEnvironmentMonitor = https.onRequest(RoomEnvironmentMonitor);