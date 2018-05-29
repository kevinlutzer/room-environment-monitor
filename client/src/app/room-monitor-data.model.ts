export class RoomMonitorData {
    temp: string;
    lux: string;
    timestamp: string;
    cpuTemp: string;
    tvoc: string;
    co2: string;

    static fromApi(data: RoomMonitorDataApiInteface): RoomMonitorData {
        const obj = new RoomMonitorData();
        obj.temp = String(data.temp);
        obj.lux = String(data.lux);
        obj.timestamp = data.timestamp;
        obj.cpuTemp = String(obj.cpuTemp);
        obj.tvoc = String(obj.tvoc);
        obj.co2 = (obj.co2);
        return obj
    }
   
}

// Api Definition of RoomMonitorData
export interface RoomMonitorDataApiInteface {
    temp: number,
    lux: number,
    timestamp: string
    cpu_temp: number
    tvoc: number,
    co2: number,
}