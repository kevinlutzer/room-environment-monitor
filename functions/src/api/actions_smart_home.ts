import * as functions from 'firebase-functions';
import * as express from 'express';
import * as cors from 'cors';


import {FullFillment} from '../controllers/actions_smart_home';

const app = express();
app.use(cors({ origin: true }));
app.post('/fullfillment', FullFillment);
app.post('')

export const IOTFullfillmentHandler = functions.https.onRequest(app)
