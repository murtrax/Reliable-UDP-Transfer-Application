
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>


//BUFFERT is the actual useful payload data
#define BUFFERT 500

//functions defined
int create_server_socket (int port);

//socket addresses defined 
struct sockaddr_in sock_serv,clt;

//structure of packet defined
struct packets{
int sq_no;
char data[BUFFERT];
};

//main function defined 
int main (int argc, char**argv){
    //creating variables which will recieve the packets

    //file descripter and socket file descripter defined
	int fd, sfd;

	//packet pointers defined and allocated memory through malloc function
 	struct packets * packet_recv0 = malloc(sizeof(struct packets));
   	struct packets * packet_recv1 = malloc(sizeof(struct packets));
	struct packets * packet_recv2 = malloc(sizeof(struct packets));
	struct packets * packet_recv3 = malloc(sizeof(struct packets));
	struct packets * packet_recv4 = malloc(sizeof(struct packets));

	//array of packets defined that will hold the correct order of the packets
	struct packets * packet_seq[100000];

	//64 bit integer values defined 
	off_t count=0, n0,n1,n2,n3,n4; 

	//char array for filename defined 
	char filename[256];

	//ack packets 
	int ack_send0,ack_send1,ack_send2,ack_send3,ack_send4=-1;

	//sock address size 
    unsigned int l = sizeof (struct sockaddr_in);		
	int frame_id=0;
    
    //if not giving two arguments then no program for you
	if (argc != 2){
		printf("Error usage : %s <port_serv>\n",argv[0]);
		return EXIT_FAILURE;
	}
    //creating the socket
    sfd = create_server_socket(atoi(argv[1]));
    

	bzero(filename,256);
	printf("Creating the output file : %s\n",filename);
    
	//opens file
	if((fd=open(filename,O_CREAT|O_WRONLY|O_TRUNC,0600))==-1){
		perror("open fail");
		return EXIT_FAILURE;
	};
    

//int varibale n0 defined that checks the status of packet recieval 	
n0=1;  

//write_var defined
int write_var=0;


	//while loop defined for packet recieval
	while(n0){

		//recieves packet client
		n0=recvfrom(sfd,packet_recv0,sizeof(*packet_recv0),0,(struct sockaddr *)&clt,&l);
		//gets sequence number
		ack_send0=packet_recv0->sq_no;
		//stores packets into correct position in packet_seq array
		packet_seq[packet_recv0->sq_no]=packet_recv0;
		//sends ack for packet
		sendto(sfd,&ack_send0,4,0,(struct sockaddr*)&clt,l);
		printf("Packet Recieved %d\n",packet_recv0->sq_no);
		//if packet not recieved, give error
		if(n0==-1){
			perror("read fails");
			return EXIT_FAILURE;
		};

		n1=recvfrom(sfd,packet_recv1,sizeof(*packet_recv1),0,(struct sockaddr *)&clt,&l);
		ack_send1=packet_recv1->sq_no;
		packet_seq[packet_recv1->sq_no]=packet_recv1;
		sendto(sfd,&ack_send1,4,0,(struct sockaddr*)&clt,l);
		printf("Packet Recieved %d\n",ack_send1);
		if(n1==-1){
			perror("read fails");
			return EXIT_FAILURE;
		};

		n2=recvfrom(sfd,packet_recv2,sizeof(*packet_recv2),0,(struct sockaddr *)&clt,&l);
		ack_send2=packet_recv2->sq_no;
		packet_seq[packet_recv2->sq_no]=packet_recv2;
		sendto(sfd,&ack_send2,4,0,(struct sockaddr*)&clt,l);
		printf("Packet Recieved %d\n",ack_send2);
		if(n2==-1){
			perror("read fails");
			return EXIT_FAILURE;
		};
		n3=recvfrom(sfd,packet_recv3,sizeof(*packet_recv3),0,(struct sockaddr *)&clt,&l);
		ack_send3=packet_recv3->sq_no;
		sendto(sfd,&ack_send3,4,0,(struct sockaddr*)&clt,l);
		packet_seq[packet_recv3->sq_no]=packet_recv3;
		printf("Packet Recieved %d\n",ack_send3);
		if(n3==-1){
			perror("read fails");
			return EXIT_FAILURE;
		};
		n4=recvfrom(sfd,packet_recv4,sizeof(*packet_recv4),0,(struct sockaddr *)&clt,&l);
		ack_send4=packet_recv4->sq_no;
		packet_seq[packet_recv4->sq_no]=packet_recv4;
		sendto(sfd,&ack_send4,4,0,(struct sockaddr*)&clt,l);
		printf("Packet Recieved %d\n",ack_send4);
		if(n4==-1){
			perror("read fails");
			return EXIT_FAILURE;
		};
		for(int x=0;x<5;x++){
			write(fd,packet_seq[write_var]->data,BUFFERT);
			write_var++;}
		
		};
		count=count+n0+n1+n2+n3+n4;
	
    
	printf("Number of bytes transferred: %lld \n",count);
	//close socket
    close(sfd);
    //close file descripter 
    close(fd);
	return EXIT_SUCCESS;
}


/* Function allowing the creation of a socket and its attachment to the system
 * Returns a file descriptor in the process descriptor table
 * bind allows its definition in the system
 */

//function creata server socket defined that creates a socket, and binds it to a port 
int create_server_socket (int port){
    int l;
	int sfd;
    
	sfd = socket(AF_INET,SOCK_DGRAM,0);
	if (sfd == -1){
        perror("socket fail");
        return EXIT_FAILURE;
	}
    
    //preparation of the address of the destination socket
	l=sizeof(struct sockaddr_in);
	bzero(&sock_serv,l);
	
	sock_serv.sin_family=AF_INET;
	sock_serv.sin_port=htons(port);
	
	//INADDR_ANY binds it all interfaces not just localhost
	sock_serv.sin_addr.s_addr=htonl(INADDR_ANY);
    
	//Assign an identity to the socket
	if(bind(sfd,(struct sockaddr*)&sock_serv,l)==-1){
		perror("bind fail");
		return EXIT_FAILURE;
	}
    
    
    return sfd;
}

