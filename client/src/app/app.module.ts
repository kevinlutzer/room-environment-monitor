import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { MatCardModule, MatCardHeader } from '@angular/material';
import { MatGridListModule } from '@angular/material/grid-list';

import { AppComponent } from './app.component';
import { MetricBoxComponent } from './metric-box.component';
import { RoomMonitorDataService } from './room-monitor-data.service';


@NgModule({
  declarations: [
    AppComponent,
    MetricBoxComponent
  ],
  imports: [
    BrowserModule,
    MatCardModule,
    MatGridListModule,
  ],
  providers: [RoomMonitorDataService],
  bootstrap: [AppComponent]
})
export class AppModule { }
