import * as functions from 'firebase-functions';
import * as admin   from 'firebase-admin';

// Initialize Firebase App
admin.initializeApp(functions.config().firebase);

// Cloud functions
export {RoomEnvironmentMonitorDeviceStatusPubsubHandler} from './api/room_environment_monitor_device_status';
export {RoomEnvironmentTelemetry, RoomEnvironmentTelemetryPubsubHandler} from './api/room_environment_telemetry';
