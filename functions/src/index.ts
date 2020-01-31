const functions = require('firebase-functions');
const admin = require('firebase-admin');
const express = require('express');
const cors = require('cors');

import { TelemetryCreatePubsubHandler, TelemetryList,
    StatusUpdatePubsubHandler, StatusList, TelemetryGetLatest} from './controllers';

export const RoomEnvironmentTelemetryPubsubTopic = 'room-environment-monitor-telemetry';
export const RoomEnvironmentStatusPubsubTopic = 'room-environment-monitor-status';

admin.initializeApp(functions.config().firebase);

const db = admin.firestore();
db.settings({timestampsInSnapshots: true});

const RoomEnvironmentMonitor = express();

var allowedOrigins = [
    'https://website-klutzer.firebaseapp.com/overview',
    'https://kevin.lutzer.ca/'
];

RoomEnvironmentMonitor.use(cors({
    origin: function(origin, callback){
      // allow requests with no origin 
      // (like mobile apps or curl requests)
      if(!origin) return callback(null, true);
      if(allowedOrigins.indexOf(origin) === -1){
        var msg = 'The CORS policy for this site does not ' +
                  'allow access from the specified Origin.';
        return callback(new Error(msg), false);
      }
      return callback(null, true);
    }
  }));

RoomEnvironmentMonitor.get('/api/telemetry/get', (req, res, next) => TelemetryGetLatest(req, res, next, db))
RoomEnvironmentMonitor.get('/api/telemetry/list', (req, res, next) => TelemetryList(req, res, next, db))
RoomEnvironmentMonitor.get('/api/status/list', (req, res, next) => StatusList(req, res, next, db))

exports.TelemetryCreatePubsubHandler = functions.pubsub.topic(RoomEnvironmentTelemetryPubsubTopic).onPublish(msg => TelemetryCreatePubsubHandler(msg, db))
exports.StatusUpsertPubsubHandler = functions.pubsub.topic(RoomEnvironmentTelemetryPubsubTopic).onPublish(msg => StatusUpdatePubsubHandler(msg, db))
exports.RoomEnvironmentMonitor = functions.https.onRequest(RoomEnvironmentMonitor);