export interface RoomEnvironmentMonitorTelemetryApiInterface {
    lux: number;
    co2: number;
    tvoc: number;
    temperature: number;
    cpu_temp: number;
    timestamp: string;
}

export class RoomEnvironmentMonitorTelemetry {

    lux: number;
    co2: number;
    tvoc: number;
    temperature: number;
    cpuTemp: number;
    timestamp: Date;

    static fromApi(data: RoomEnvironmentMonitorTelemetryApiInterface): RoomEnvironmentMonitorTelemetry {
        let obj = new RoomEnvironmentMonitorTelemetry();
        obj.lux = data.lux;
        obj.co2 = data.co2;
        obj.tvoc = data.tvoc;
        obj.temperature = data.temperature;
        obj.cpuTemp = data.cpu_temp;
        obj.timestamp = new Date(data.timestamp)
        return obj;
    }
}
