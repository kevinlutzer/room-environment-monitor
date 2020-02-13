# constants
project=iot-klutzer
region=us-central1
registry=devices-klutzer
events=room-monitor-telemetry

# create the registries
gcloud iot registries create $registry --project=$project --region=$region --event-notification-logger=topic=projects/$project/topics/$events
