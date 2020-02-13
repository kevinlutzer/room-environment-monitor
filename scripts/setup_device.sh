if [ $# -eq 0 ]
  then
    echo "The name of the device must be passed as argument 1"
    exit 0
fi

# constants
defaultConfigDir=certs
project=iot-klutzer
region=us-central1
registry=devices-klutzer
device=$1
configDirName=certs


# # create directory if it doen't exist to hold auth files
mkdir -p $configDirName

# # generate public/private keys for device
openssl req -x509 -newkey rsa:2048 -keyout $configDirName/rsa_private.pem -nodes -out $configDirName/rsa_cert.pem

# create a new device for public/private keys
gcloud iot devices create $device --project=$project --region=$region --registry=$registry --public-key path=$configDirName/rsa_cert.pem,type=rs256

# # needed ssl cert 
wget https://pki.google.com/roots.pem -P $configDirName
