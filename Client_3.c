#include <stdio.h>
#include <stdlib.h>

// Time function, sockets, htons... file stat
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <math.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

//buffer size
#define BUFFERT 500

int create_client_socket (int port, char* ipaddr);

// Socket for Server Side
struct sockaddr_in server_socket;

// time value to store secs and mili secs
struct timeval time;

// packet struct to store sequence number and array for payload
struct packets{

int seq_number;
char payload[BUFFERT];

};

int main (int argc, char**argv){
	
	// file descriptor AND SOCKET DISCRIPTOR
    int sfd,fd;
   
    long int size;
    int no_packets;
    int total_acks;

	// 5 PACKETS TO SEND	
    struct packets packet_s0,packet_s1,packet_s2,packet_s3,packet_s4;

	//buffer to store data 
    char buf[BUFFERT];

    off_t count=0, m, file_size;
    
	// to check if recvfrom() is successfull
    long int n0,n1,n2,n3,n4;

	// size of sockaddr_in
    int l=sizeof(struct sockaddr_in);
	struct stat buffer; // stats for file, to get size of file
    
	if (argc != 4){
		printf("Error usage : %s <ip_serv> <port_serv> <filename>\n",argv[0]);
		return EXIT_FAILURE;
	}
    //creating the socket
    sfd=create_client_socket(atoi(argv[2]), argv[1]);
    
	if ((fd = open(argv[3],O_RDONLY))==-1){
		perror("open fail");
		return EXIT_FAILURE;
	}
    
	//file size
	if (stat(argv[3],&buffer)==-1){
		perror("stat fail");
		return EXIT_FAILURE;
	}
	else
		file_size=buffer.st_size;
    	// counting the total no of packets
	no_packets=ceil((float)file_size/500);
	//declare array of the same size 
	struct packets packets_array[no_packets];
	//initializing ack array	
	int acks_recv[no_packets];
	for(int a = 0; a < no_packets; a = a+1 ){
      		acks_recv[a]=-1;
   	}


    //adding packets into the array
    for(int var=0;var<no_packets;var++){
    n0=read(fd,packets_array[var].payload,BUFFERT);}
    //setting sq numbers
    for(int var_1=0;var_1<no_packets;var_1++){
    	packets_array[var_1].seq_number=var_1;
	}

    total_acks = 5;
  
	
    
    // window soze to check how many packets are sent
    int pack_window=0;
// ack window to see how many acks are received
    int ack_window=0;
    

//the sending of packets    
    while(pack_window!=no_packets){
	total_acks=0;
	for(int u=0;u<5;u++){
    	if(sendto(sfd,&packets_array[pack_window],sizeof(packets_array[pack_window]),0,(struct sockaddr*)&server_socket,l)==-1){
			perror("send error");
			return EXIT_FAILURE;}
		pack_window++;
	}
	while(1){
		recvfrom(sfd,&acks_recv[ack_window],4,0,(struct sockaddr*)&server_socket,&l);
		if(acks_recv[ack_window]==ack_window){
			printf("Ack Recieved %d\n",acks_recv[ack_window]);
			total_acks++;
			ack_window++;}
		
		else{if(sendto(sfd,&packets_array[ack_window],sizeof(packets_array[ack_window]),0,(struct sockaddr*)&server_socket,l)==-1){
			perror("send error1");
			return EXIT_FAILURE;}}
		if(total_acks==5){
			break;}

	}

}
		
    
	//printf("Number of bytes transferred: %lld\n",count);
	printf("On a total size of: %lld \n",file_size);
	printf("For a total duration of: %ld.%d \n",delta.tv_sec,delta.tv_usec);
    
    close(sfd);
    close(fd);
    	return EXIT_SUCCESS;}



/* Function allowing the creation of a socket
 * Returns a file descriptor
 */
int create_client_socket (int port, char* ipaddr){
    	int l;
	int sfd;
    
	sfd = socket(AF_INET,SOCK_DGRAM,0);
	if (sfd == -1){
        perror("socket fail");
        return EXIT_FAILURE;
	}
    
    //preparation of the address of the destination socket
	l=sizeof(struct sockaddr_in);
	bzero(&server_socket,l);
	
	server_socket.sin_family=AF_INET;
	server_socket.sin_port=htons(port);
	time.tv_sec = 5;
    	time.tv_usec = 0;
    if (inet_pton(AF_INET,ipaddr,&server_socket.sin_addr)==0){
		printf("Invalid IP adress\n");
		return EXIT_FAILURE;
	}
	
	if (setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO,&time,sizeof(time)) < 0) {
    		perror("Error");
	}
    
    return sfd;
}