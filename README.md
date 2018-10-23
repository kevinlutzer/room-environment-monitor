# room environment monitor

A small iot device built from the raspberry pi and some adafruit sensors.

# Compilation on Work Station
`GOARM=6 GOARCH=arm GOOS=linux go build main.go`

# Crontab

``` bash 
*/15 * * * * /usr/bin/wget http://192.168.1.140:8080/publish-sensor-data-snapshot - O /dev/null
* * * * * /usr/bin/wget http://192.168.1.140:8080/subscribe-iot-config -O /dev/null
*/5 0 * * * /usr/bin/wget http://192.168.1.140:8080/publish-device-status -O /dev/null
``` 