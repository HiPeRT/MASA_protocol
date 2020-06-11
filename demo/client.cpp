/*
    C socket client example
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
#include "../support.h"


void prepare_message(MasaMessage *m, int idx)
{
    m->cam_idx = idx;
    m->t_stamp_ms = time_in_ms();
    m->num_objects = 5;

    m->objects.clear();
    RoadUser r1{.3f,.4f,0,1,C_car};
    RoadUser r2{.5f,.6f,1,2,C_bus};
    RoadUser r3{.7f,.8f,2,3,C_bycicle};
    RoadUser r4{.9f,.10f,3,4,C_motorbike};
    RoadUser r5{.11f,.12f,4,5,C_person};
    m->objects.push_back(r1);
    m->objects.push_back(r2);
    m->objects.push_back(r3);
    m->objects.push_back(r4);
    m->objects.push_back(r5);

    m->lights.clear();
    TrafficLight l1{.1f,.2f,0,L_green,15};
    TrafficLight l2{.2f,.3f,2,L_red,15};
    m->lights.push_back(l1);
    m->lights.push_back(l2);

}

void usage(char * progname)
{
    printf("Usage: %s [-h] [-p <port>] [-u <server url>]\n", progname);
}

int main(int argc, char *argv[])
{
    // See file "protocol_ports.md"
    const char * ip = "127.0.0.1";
    int port = 8888;

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

                case 'u':
                    if(++i >= argc)
                    {
                        printf("\nError. Missing url param\n\n");
                        usage(argv[0]);
                        exit(EXIT_FAILURE);
                    }

                    ip = argv[i];

                    break;

                default:
                    usage(argv[0]);
                    exit(EXIT_FAILURE);
            }
        }
    }

    Communicator<MasaMessage> Comm(SOCK_DGRAM); // Specialize for your "message". See also "messages.hpp"

    Comm.open_client_socket((char *) ip, port); 

    MasaMessage *m = new MasaMessage;

    for (int i=0; i<10; i++)
    {
        prepare_message(m,i);
        std::stringbuf s;
        Comm.serialize_coords(m,&s);

        std::cout<<s.str()<<std::endl;
        std::cout<<s.str().length()<<std::endl;

        Comm.send_message(m, port);
        sleep(1);
    }

    return 0;
}
