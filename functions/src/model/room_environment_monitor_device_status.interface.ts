export enum Status {
    Active = "Active",
    Unactive = "Unactive",
}

export interface RoomEnvironmentMonitorDeviceStatusPubsubMessageInterface {
    status: string;
    cpu_temp: number;
}

export interface RoomEnvironmentMonitorDeviceStatusInterface {
    status: Status;
    cpuTemp: number;
}

export function Convert(data: RoomEnvironmentMonitorDeviceStatusPubsubMessageInterface): RoomEnvironmentMonitorDeviceStatusInterface
{
    return {
        status: Status[data.status],
        cpuTemp: data.cpu_temp || 0,
    }
}