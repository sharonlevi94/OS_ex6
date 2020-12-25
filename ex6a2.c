//client program:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>        // for read/write/close
#include <sys/types.h>     /* standard system types       */
#include <netinet/in.h>    /* Internet address structures */
#include <sys/socket.h>    /* socket interface functions  */
#include <netdb.h>         /* host to IP resolution       */
//========================================================================
#define  YES 1
#define  NO  0
#define  WANT_TO_CONNECT 2
const int FINISH= -1;
const int CAN_START =1;
//========================================================================
void terminate(char *);
//========================================================================
int main(int argc, char *argv[])
{
	 if (argc < 4) {
	        fprintf(stderr, "Missing server name \n");
	        exit(EXIT_FAILURE) ;
	    }

	srand               (atoi(argv[1]));
    int                 rc;            /* system calls return value storage */
    int                 my_socket;
    int                 rand_num;
    int                 answer=1;
    struct addrinfo     con_kind,
                        *addr_info_res ;
    int                 nums_sent=0,nums_deleted=0;

    memset(&con_kind, 0, sizeof(con_kind)) ;
    con_kind.ai_family = AF_UNSPEC ;        // AF_INET, AF_INET6
    con_kind.ai_socktype = SOCK_STREAM ;

    if ((rc = getaddrinfo(argv[2], argv[3],//<-server port number -argv[3]
                                     &con_kind,
                                     &addr_info_res) != 0)) {
       fprintf(stderr, "(getaddrinfo() failed %s\n", gai_strerror(rc)) ;
       exit(EXIT_FAILURE) ;
    }
    my_socket = socket(addr_info_res->ai_family,        // PF_INET
                       addr_info_res->ai_socktype,      // SOCK_STREAM
                       addr_info_res->ai_protocol);
                                         // 0 = u decide which protocal to use
    if (my_socket < 0) {
        perror("socket: allocation failed");
        exit(EXIT_FAILURE) ;
    }

    rc = connect(my_socket,
                 addr_info_res ->ai_addr,    // addr of server: IP+PORT
                 addr_info_res ->ai_addrlen) ;
    if (rc) {
        perror("connect failed:");
        exit(EXIT_FAILURE) ;
    }

/*
    //missed: send message to server and wait for feedback to start
    rc = write(my_socket,&atoi(argv[1]),sizeof(int));
    while((rc=read(my_socket,&answer,sizeof(int))<=0)){}
  */
    //loop of send random numbers to server:
    while(answer!=FINISH){
    	rand_num=rand()%10;
    	rc = write(my_socket,&rand_num,sizeof(int));
    	nums_sent++;
    	if(rc<0)
    		terminate("cannot write to server\n");
    	rc=read(my_socket,&answer,sizeof(int));
    	if(rc<0){
    		break;
    		terminate("cannot read from server\n");
    	}
    	if(answer==YES){
    		nums_deleted++;
    	}
    }
    close(my_socket);
    freeaddrinfo(addr_info_res) ;
    printf ("%d %d\n",nums_sent,nums_deleted);
    return EXIT_SUCCESS;
}
//=========================================================================
//this function terminate the program if there is an error in fork
void terminate(char *error_message){
    perror (error_message);
    exit(EXIT_FAILURE);
}
