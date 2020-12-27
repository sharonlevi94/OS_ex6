
//GCDserver program:
//server program:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>        // for memset
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
//========================================================================
int                fd; //closed in signal handler
fd_set             rfd;
#define            MAX_LEN 20
//========================================================================
void terminate(char *);
void catch_sigint(int sig_num);
int get_num(char []);
int find_gcd (int,int);
//========================================================================
int main(int argc,char* argv[])
{
    if (argc < 2) {
                fprintf(stderr, "Missing port \n");
                exit(EXIT_FAILURE) ;
            }
    signal(SIGINT,catch_sigint);
    int                rc;                // return code
    int                main_socket;
    int                serving_socket;
    fd_set             c_rfd;

    struct sockaddr_storage her_addr;
    socklen_t her_addr_size;
    struct addrinfo con_kind,
                            *addr_info_res ;
    int               gcd_prospect1, gcd_prospect2;
    int               gcd_to_return;
    char              read_string[MAX_LEN];

    memset(&con_kind, 0, sizeof con_kind) ;
    con_kind.ai_family = AF_UNSPEC ;
    con_kind.ai_socktype = SOCK_STREAM ;
    con_kind.ai_flags = AI_PASSIVE ;        // system will fill my IP

    if ((rc = getaddrinfo(NULL,            // NULL = you set IP address
                          argv[1],
                          &con_kind,
                          &addr_info_res)) != 0) {
       fprintf(stderr, "getaddrinfo() failed %s\n", gai_strerror(rc)) ;
       exit(EXIT_FAILURE) ;
    }

    main_socket = socket(addr_info_res -> ai_family,
                         addr_info_res -> ai_socktype,
                         addr_info_res -> ai_protocol);
    if (main_socket < 0) {
        perror("socket: allocation failed");
        exit(EXIT_FAILURE) ;
    }

    rc = bind(main_socket, addr_info_res-> ai_addr,
                                          addr_info_res-> ai_addrlen);
    if (rc) {
        perror("bind failed");
        exit(EXIT_FAILURE) ;
    }

    rc = listen(main_socket, 3); //listen to clients
    if (rc) {
        perror("listen failed");
        exit(EXIT_FAILURE) ;
    }

    her_addr_size = sizeof(her_addr);

    FD_ZERO(&rfd);
    FD_SET(main_socket, &rfd);


    while (1) {
        c_rfd = rfd;
        rc = select(getdtablesize(),
                    &c_rfd,
                    NULL, NULL, (struct timeval *) NULL);
        if (FD_ISSET(main_socket, &c_rfd)) {
            serving_socket = accept(main_socket,
                                 (struct sockaddr *)&her_addr,
                                 &her_addr_size);
            if (serving_socket >= 0) {
               FD_SET(serving_socket, &rfd);
            }
        }
        for (fd=main_socket +1; fd < getdtablesize(); fd++)
            if(FD_ISSET(fd,&c_rfd)){
                rc = (int)read(fd, &read_string, MAX_LEN);
                gcd_prospect1 = atoi(read_string);
                gcd_prospect2 = get_num(read_string);
                gcd_to_return =find_gcd(gcd_prospect1, gcd_prospect2);
                rc = (int)write(fd ,&gcd_to_return, sizeof(int));
            }
    }
    return(EXIT_SUCCESS) ;
}
//========================================================================
//this function terminate the program if there is an error in fork
void terminate(char *error_message){
    perror (error_message);
    exit(EXIT_FAILURE);
}
//-------------------------------------------------------------------------
int find_gcd (int n1,int n2){
    int gcd = 0,i;
    for(i=1; i <= n1 && i <= n2; ++i)
        {
          // Checks if i is factor of both integers
          if(n1%i==0 && n2%i==0){
             gcd = i;
          }
        }
    return gcd;
}
//-------------------------------------------------------------------------
int get_num(char make_int[]){
    char *token;
    char *search = " ";
    int num_to_return;
    // to point to second num in the string
    token = strtok(make_int, search);
    token = strtok(NULL, search);
    num_to_return = atoi(token);
    return num_to_return;
}
//-------------------------------------------------------------------------
void catch_sigint(int sig_num){
    close(fd);
     FD_CLR(fd,&rfd);
        exit(EXIT_SUCCESS);
}
