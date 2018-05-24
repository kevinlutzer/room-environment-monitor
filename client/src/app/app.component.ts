import { Component } from '@angular/core';
import { RoomMonitorDataService } from './room-monitor-data.service';
@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent {
  constructor( roomMonitorService: RoomMonitorDataService) {}
}
