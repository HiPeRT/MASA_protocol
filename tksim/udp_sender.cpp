#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define RECEIVER_PORT 8001
#define SENDER_PORT 8002
#define BUFSIZE 1

int main(int argc, char **argv)
{
	const char *server = "192.168.100.2";
	struct sockaddr_in myaddr;	/* our address */
	struct sockaddr_in remaddr;	/* remote address */
	socklen_t addrlen = sizeof(remaddr);		/* length of addresses */
	int recvlen;			/* # bytes received */
	int fd;				/* our socket */
	int msgcnt = 0;			/* count # of messages we received */
	uint8_t buf = 0;	/* receive buffer */


	/* create a UDP socket */

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket\n");
		return 0;
	}

	/* bind the socket to any valid IP address and a specific port */

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(RECEIVER_PORT);
	memset((char *) &remaddr, 0, sizeof(remaddr));
	remaddr.sin_family = AF_INET;
	remaddr.sin_port = htons(SENDER_PORT);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}

	if (inet_aton(server, &remaddr.sin_addr)==0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}


	for (;;) {
		buf++;
		printf("sending message \"%d\"\n", buf);
		if (sendto(fd, &buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, addrlen) < 0)
			perror("sendto");
		printf("waiting on port %d\n", RECEIVER_PORT);
		recvlen = recvfrom(fd, &buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
		if (recvlen > 0) {
			//buf[recvlen] = 0;
			printf("received message: \"%d\" (%d bytes)\n", buf, recvlen);
		}
		else
			printf("something went wrong!\n");
		//sprintf(buf, "ack %d", msgcnt++);
		
	}
	/* never exits */
}
