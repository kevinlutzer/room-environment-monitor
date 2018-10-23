export enum Status {
    Unactive = "Unactive",
}

export interface RoomEnvironmentMonitorDeviceStatusPubsubMessageInterface {
    cpu_temp: number;
}

export interface RoomEnvironmentMonitorDeviceStatusInterface {
    cpuTemp: number;
}

export function Convert(data: RoomEnvironmentMonitorDeviceStatusPubsubMessageInterface): RoomEnvironmentMonitorDeviceStatusInterface
{
    return {
        cpuTemp: data.cpu_temp || 0,
    }
}