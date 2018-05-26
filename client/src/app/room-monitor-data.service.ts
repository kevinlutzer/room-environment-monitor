import {Injectable} from '@angular/core';
import {RoomMonitorData} from './room-monitor-data.model';
import {Observable, of as Observableof} from 'rxjs';

@Injectable()
export class RoomMonitorDataService {
    get(): Observable<RoomMonitorData> {
        return Observableof(RoomMonitorData.fromApi({
            timestamp: "Wed May 23 2018 22:06:52 GMT-0600 (CST)",
            temp: 32.5,
            lux: 2000,
            cpu_temp: 45.3,
            tvoc: 1,
            co2: 1000,
        }));
    }
}