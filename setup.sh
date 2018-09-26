project=iot-klutzer
region=us-central1
registry=devices-klutzer
device=room-monitor-rs256-device
events=room-monitor-telemetry		
configDirName=certs

# create the registries
gcloud iot registries create $registry --project=$project --region=$region --event-notification-config=topic=projects/$project/topics/$events

# create directory if it doen't exist to hold auth files
mkdir -p $configDirName

# generate public/private keys for device
openssl req -x509 -newkey rsa:2048 -keyout $configDirName/rsa_private.pem -nodes -out $configDirName/rsa_cert.pem

# create a new device for public/private keys
gcloud iot devices create $device --project=$project --region=$region --registry=$registry --public-key path=$configDirName/rsa_cert.pem,type=rs256

# needed ssl cert 
wget https://pki.google.com/roots.pem -P $configDirName
