export class RoomMonitorData {
    temp: number;
    lux: number;
    timestamp: Date;
    cpuTemp: number;
    tvoc: number;
    co2: number;

    static fromApi(data: RoomMonitorDataApiInteface): RoomMonitorData {
        const obj = new RoomMonitorData();
        obj.temp = data.temp;
        obj.lux = data.lux;
        obj.timestamp = new Date(obj.timestamp);
        obj.cpuTemp = obj.cpuTemp;
        obj.tvoc = obj.tvoc;
        obj.co2 = obj.co2;
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