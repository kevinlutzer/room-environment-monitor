import * as functions from 'firebase-functions';

import {PubsubHandler} from '../controllers/room_environment_monitor_device_status';
import {RoomEnvironmentMonitorDeviceStatusPubsubTopic} from '../config';

export const RoomEnvironmentMonitorDeviceStatusPubsubHandler = functions.pubsub.topic(RoomEnvironmentMonitorDeviceStatusPubsubTopic).onPublish(PubsubHandler)
