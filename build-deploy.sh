GOARM=5 GOARCH=arm GOOS=linux go build -o main main.go # Build for raspi architecture specifically the raspberry pi w arm processors
scp main pi@192.168.1.111:/home/pi/
rm main
ssh -t pi@192.168.1.111 "cd ~/ && ./main"
