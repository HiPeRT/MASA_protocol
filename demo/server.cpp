/*
    C socket server example, handles multiple clients using threads
*/

#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h>   //for threading , link with lpthread
#include <sys/types.h>
#include <sys/stat.h>
#include <string>

#include <masa.hpp>


void write_message(MasaMessage *m)
{
    printf("Received message from %d at %lu with %lu road users and %lu traffic lights \n",
        m->cam_idx, m->t_stamp_ms, m->objects.size(), m->lights.size());

    printf("Objects: \n");
    for (unsigned int i = 0; i < m->objects.size(); i++)
    {
        printf("lat %f lon %f speed %d orient %d class %d\n",
            m->objects.at(i).latitude,
            m->objects.at(i).longitude,
            m->objects.at(i).speed,
            m->objects.at(i).orientation,
            m->objects.at(i).category);
    }

    printf("Lights: \n");
    for (unsigned int i = 0; i < m->lights.size(); i++)
    {
        printf("lat %f lon %f orient %d status %d time_to_change %d\n",
            m->lights.at(i).latitude,
            m->lights.at(i).longitude,
            m->lights.at(i).orientation,
            m->lights.at(i).status,
            m->lights.at(i).time_to_change);
    }


}

//the thread function
void *connection_handler(void *);

void usage(char * progname)
{
    printf("Usage: %s [-h] [-p <port>]\n", progname);
}

int main(int argc, char *argv[])
{

    int socket_desc, client_sock, c, *new_sock, port = 8888;
    struct sockaddr_in client;

    // Parse args
    for(int i=1; i<argc; i++)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 'h':
                    usage(argv[0]);
                    exit(EXIT_SUCCESS);
                    break;

                case 'p':

                    if(++i >= argc)
                    {
                        printf("\nError. Missing port number param\n\n");
                        usage(argv[0]);
                        exit(EXIT_FAILURE);
                    }
                    
                    port = atoi(argv[i]);
                    if(port == 0)
                    {
                        printf("\nInvalid port number param '%s'\n\n", argv[i]);
                        usage(argv[0]);
                        exit(EXIT_FAILURE);
                    }

                    break;

                default:
                    usage(argv[0]);
                    exit(EXIT_FAILURE);
            }
        }
    }

    Communicator<MasaMessage> Comm(SOCK_DGRAM); // Specialize for your "message". See also "messages.hpp"

    Comm.open_server_socket(port); // Replace this with your port. See file "protocol_ports.md"
    socket_desc = Comm.get_socket();

    //Accept and incoming connection
    puts("Waiting for incoming connections...");

    if (Comm.SOCKET_MODE == SOCK_STREAM)
    {
        c = sizeof(struct sockaddr_in);
        while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
        {
            puts("Connection accepted");

            pthread_t sniffer_thread;
            new_sock = (int *)malloc(1);
            *new_sock = client_sock;

            if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0)
            {
                perror("could not create thread");
                return 1;
            }

            //Now join the thread , so that we dont terminate before the thread
            //pthread_join( sniffer_thread , NULL);
            puts("Handler assigned");
        }

        if (client_sock < 0)
        {
            perror("accept failed");
            return 1;
        }
    }
    else
    {
        MasaMessage *m = new MasaMessage;

        //Receive a message from client
        while (Comm.receive_message(socket_desc,m)==0)
        {
            write_message(m);
        }
    }

    return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    Communicator<MasaMessage> myComm(SOCK_STREAM);  // Specialize for your "message". See also "messages.hpp"
                                                          
    MasaMessage *m = new MasaMessage;

    //Get the socket descriptor
    int sock = *(int *)socket_desc;
    int read_size = 0;

    //Receive a message from client
    while (myComm.receive_message(sock,m)==0)
    {
        write_message(m);
    }

    if (read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    //Free the socket pointer
    free(socket_desc);


    return 0;
}
