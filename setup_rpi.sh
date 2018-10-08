#run on the raspberry pi

if [ $# -eq 0 ]
  then
    echo "IP address needs to be supplied as arugment 1"
fi
exit 0

ssh -t pi@$1 "sudo systemctl stop iot-server"
GOARM=5 GOARCH=arm GOOS=linux go build -o main main.go # Build for raspi architecture specifically the raspberry pi w arm processors
scp main pi@$1:/home/pi/
rm main
scp iot-server.service pi@$1:/home/pi/
scp install_rpi.sh pi@$1:/home/pi/
ssh -t pi@$1 "sudo mv iot-server.service /etc/systemd/system && 
sudo systemctl enable iot-server &&
sudo systemctl start iot-server"