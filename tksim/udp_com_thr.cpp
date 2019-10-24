#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define WAITINGBUFF
#define PORT 8001

struct connection_t{
	int udpfd;
	struct sockaddr_in myaddr;
	struct sockaddr_in remaddr;
	socklen_t addrlen;
    fd_set rset;
};
	

struct PACKAGE{
	uint8_t msgcnt;		//message_count
	uint8_t addhere;	//variable where to do +1
	bool ack;
};

void *receiver_thread(void * args){
	connection_t *con = (connection_t *)args;
	PACKAGE s;
	char tmp1[sizeof(PACKAGE)];
	int recvlen;
	int nready;
	struct timeval tv;
	tv.tv_sec = 10;
	tv.tv_usec = 50000;
	while(true){
		FD_SET((int)con->udpfd,&con->rset);
		nready = select(((int)con->udpfd+1), &con->rset, NULL, NULL, &tv);
		if (nready == 0) {
			printf("Timeout! Nessun dato dopo 10,5 secondi.\n");
		}
		if(FD_ISSET((int)con->udpfd,&con->rset)){
			//recvlen = recvfrom((int)con->udpfd, &tmp1, sizeof(PACKAGE), 0, (struct sockaddr *)&con->remaddr, (socklen_t *)&con->addrlen);
			recvlen = recv((int)con->udpfd,&tmp1,sizeof(PACKAGE),0);
			if (recvlen > 0) {
			memcpy(&s, tmp1, sizeof(PACKAGE));
			printf("msg: %d\treceived message: %d-%d\n\n", s.msgcnt,s.addhere,s.ack );
			}
			FD_ZERO(&con->rset);
		}

	}
		
}


int main(int argc, char **argv)
{
	const char *server = "192.168.100.2";
	connection_t con;
	con.addrlen = sizeof(con.remaddr);
	int recvlen;			/* # bytes received */
	int msgcnt = 0;			/* count # of messages we received */
	PACKAGE s,received;
	s.msgcnt = 0;
	s.addhere = 0;
	char tmp1[sizeof(PACKAGE)];
	char tmp2[sizeof(PACKAGE)];
	printf("%lu\n",sizeof(PACKAGE));
	/* create a UDP socket */
	if ((con.udpfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket\n");
		return 0;
	}
	//local address
	memset((char *)&con.myaddr, 0, sizeof(con.myaddr));
	con.myaddr.sin_family = AF_INET;
	con.myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	con.myaddr.sin_port = htons(PORT);

	//remote address
	memset((char *) &con.remaddr, 0, sizeof(con.remaddr));
	con.remaddr.sin_family = AF_INET;
	con.remaddr.sin_port = htons(PORT);

	if (bind(con.udpfd, (struct sockaddr *)&con.myaddr, sizeof(con.myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}
	FD_ZERO(&con.rset);
	
	//Transform IP address in binary form
	if (inet_aton(server, &con.remaddr.sin_addr)==0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}
	
	pthread_t thread;
	pthread_create(&thread,NULL,receiver_thread,(void*)&con);

	s.ack=1;
	for (;;) {
		s.addhere++;
		s.msgcnt++;
		printf("msgcount :%d\tsending message:%d\n", s.msgcnt,s.addhere);
		if (sendto(con.udpfd, &s, sizeof(PACKAGE), 0, (struct sockaddr *)&con.remaddr, con.addrlen) > 0){
			sleep(1);
			//add to waiting ack list
		}
		else		
			perror("sendto");
		
	}
	/* never exits */
}
