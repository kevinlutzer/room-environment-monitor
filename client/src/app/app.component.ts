import { Component, OnInit } from '@angular/core';
import { Observable } from 'rxjs';

import { RoomMonitorDataService } from './room-monitor-data.service';
import { RoomMonitorData } from './room-monitor-data.model';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit {
  constructor( private roomMonitorService: RoomMonitorDataService) {}

  data$: Observable<RoomMonitorData>;
  loading: boolean;

  ngOnInit(){
    this.data$ = this.roomMonitorService.data$;
    this.data$.subscribe(val => console.log(val));
    this.loading = this.roomMonitorService.loading;
  }
}
