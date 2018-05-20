project=personal-website-klutzer
region=us-central1
registry=klutzer-devices
device=raspberry-pi-room-monitor-rs256-device
events=room-monitor-telemetry		
configDirName=go-server/certs/internal/server

# run on any device
function create_iot_registry {
    gcloud beta iot registries create $registry \
        --project=$project \
        --region=$region \
        --event-pubsub-topic=projects/$project/topics/$events
}

# run only on the iot device or 
function create_iot_device {

    # create directory if it doen't exist to hold auth files
    mkdir -p $configDirName

    # generate public/private keys for device
    openssl req -x509 -newkey rsa:2048 -keyout $configDirName/rsa_private.pem -nodes -out $configDirName/rsa_cert.pem

    # create a new device for public/private keys
    gcloud beta iot devices create $device \
        --project=$project \
        --region=$region \
        --registry=$registry \
        --public-key path=$configDirName/rsa_cert.pem,type=rs256

    # needed ssl cert 
    wget https://pki.google.com/roots.pem -P $configDirName
}

# entry point into application
"$@"