GOARM=6 GOARCH=arm GOOS=linux go build -o main main.go # Build for raspi architecture
scp main pi@192.168.1.111:/home/pi/
ssh -t pi@192.168.1.111 "./home/pi/main"