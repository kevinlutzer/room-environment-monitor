# Takes in the ip address as the first parameter
if [ $# -eq 0 ]
  then
    echo "IP address needs to be supplied as arugment 1"
fi
exit 0

GOARM=5 GOARCH=arm GOOS=linux go build -o main main.go # Build for raspi architecture specifically the raspberry pi w arm processors
scp main pi@$1:/home/pi/
rm main
ssh -t pi@$1 "cd ~/ && ./main"
