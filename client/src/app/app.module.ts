import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppComponent } from './app.component';
import { MetricBoxComponent } from './metric-box.component';
import { RoomMonitorDataService } from './room-monitor-data.service';

@NgModule({
  declarations: [
    AppComponent,
    MetricBoxComponent
  ],
  imports: [
    BrowserModule
  ],
  providers: [RoomMonitorDataService],
  bootstrap: [AppComponent]
})
export class AppModule { }
