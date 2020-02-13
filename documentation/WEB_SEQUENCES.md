title Data Collection and Transmission Sequence (Raspberry PI)

Raspberry Pi->+Air Sensor: get air quality data
Air Sensor-->-Raspberry Pi: data
Raspberry Pi->+Temperature Sensor: get temperature and humidity
Temperature Sensor-->-Raspberry Pi: data
Raspberry Pi->+Google IOT Core: publishes data as message
Google IOT Core-->-Raspberry Pi: reciept for recieved message


title Data Store and Processing Sequence (Google Cloud Platform)

Google IOT Core->Google Pubsub: publish data from raspberry pi
Google Pubsub-->-Google IOT Core: success
Google Pubsub->+Google Function: recieve pub/sub message with data
Google Function->+Firestore: writes data to database
Firestore-->-Google Function: success
Google Function-->-Google Pubsub: sucess