import * as functions from 'firebase-functions';
import * as express from 'express';
import * as cors from 'cors';


import {FullFillment} from '../controllers/iot_fullfillment';

const app = express();
app.use(cors({ origin: true }));
app.post('/', FullFillment);

export const IOTFullfillmentHandler = functions.https.onRequest(app)
