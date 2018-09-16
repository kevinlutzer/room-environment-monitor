"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class RoomEnvironmentMonitorTelemetry {
    static fromApi(data) {
        let obj = new RoomEnvironmentMonitorTelemetry();
        obj.lux = data.lux;
        obj.co2 = data.co2;
        obj.tvoc = data.tvoc;
        obj.temperature = data.temperature;
        obj.cpuTemp = data.cpu_temp;
        obj.timestamp = new Date(data.timestamp);
        return obj;
    }
}
exports.RoomEnvironmentMonitorTelemetry = RoomEnvironmentMonitorTelemetry;
//# sourceMappingURL=room-environment-monitor-telemetry.api.model.js.map