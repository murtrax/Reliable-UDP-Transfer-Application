# Reliable-UDP-Transfer-Application
Made the UDP transfer protocol reliable by having each UDP package labeled with a number, once a package is received an "ack" is sent to the server-side, if an "ack" is not received for a package, it is retransmitted


Server side:
gcc Server_3.c -o Server
./Server <port number>/<file location>   e.g:
./Server 5500/Home

Client Side:
gcc Client_3.c -o Client -lm
./Client <IP of server> <port number> /<file location>/<file name>   e.g
./Client 192.168.8.103 5500 /Home/user/video.mp4
