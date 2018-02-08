project=klutzer-iot
region=us-central1
registry=example-registry
device=my-rs256-device
device2=my-es256-device
mysub=my-sub
events=events

# gcloud beta iot registries create $registry \
# 	--project=$project \
# 	--region=$region \
# 	--event-pubsub-topic=projects/$project/topics/$events

# openssl req -x509 -newkey rsa:2048 -keyout rsa_private.pem -nodes -out rsa_cert.pem

# gcloud beta iot devices create $device \
# 	--project=$project \
# 	--region=$region \
# 	--registry=$registry \
# 	--public-key path=rsa_cert.pem,type=rs256

# openssl ecparam -genkey -name prime256v1 -noout -out ec_private.pem
# openssl ec -in ec_private.pem -pubout -out ec_public.pem

# gcloud beta iot devices create $device2 \
# 	--project=$project \
# 	--region=$region \
# 	--registry=$registry \
# 	--public-key path=ec_public.pem,type=es256

# sudgcloud beta pubsub subscriptions create projects/$project/subscriptions/$mysub --topic=$events
# gcloud beta pubsub subscriptions create projects/klutzer-iot/subscriptions/my-sub --topic=my-sub
# gcloud beta pubsub subscriptions pull --auto-ack projects/klutzer-iot/subscriptions/my-sub

gcloud beta pubsub subscriptions create projects/$project/subscriptions/$mysub --topic=$events
gcloud beta pubsub subscriptions pull --auto-ack projects/$project/subscriptions/$mysub

# wget https://pki.google.com/roots.pem
