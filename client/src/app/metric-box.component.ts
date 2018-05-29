import { Component, Input } from '@angular/core';

@Component({
  selector: 'metric-box',
  templateUrl: './metric-box.component.html',
  styleUrls: ['./metric-box.component.scss']
})
export class MetricBoxComponent {
  @Input() name: string;
  @Input() value: string;
}
