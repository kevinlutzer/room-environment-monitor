import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { MatCardModule, MatCardHeader } from '@angular/material';
import { MatGridListModule } from '@angular/material/grid-list';

import { AppComponent } from './app.component';
import { MetricBoxComponent } from './metric-box.component';
import { RoomMonitorDataService } from './room-monitor-data.service';
import { FlexLayoutModule } from '@angular/flex-layout';

@NgModule({
  declarations: [
    AppComponent,
    MetricBoxComponent
  ],
  imports: [
    BrowserModule,
    MatCardModule,
    MatGridListModule,
    FlexLayoutModule,
    CommonModule
  ],
  providers: [RoomMonitorDataService],
  bootstrap: [AppComponent]
})
export class AppModule { }
