 //ex6a1: server
// this program creates the random numbers array and the socket.
// after checking that we have 3 different number makers, we look for
// numbers to delete. when receive a number, it checks if the number exists in the array.
// if so- it will bw deleted and replaced with '-1'. if not, continue;
// when the array is all set to be -1, we finish .
// the output will be- how much time it took.
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
//========================================================================
#define  ARR_SIZE  10
#define RAND_RANGE 10
int YES =1;
int NO =0;
#define NUM_OF_CLIENTS 3
#define NOT_EXIST -1
int FINISH=-1;
//========================================================================
void build_array(int []);
void terminate(char *);
int find_location(int arr[],int num);
void delete_number(int* arr , int location);
//========================================================================
int main(int argc,char* argv[])
{
	srand              (0);
	int                nums_deleted = 0;
    int                rc;                // return code
    int                main_socket;
    int                serving_socket;
    int                fd;
    fd_set             rfd;
    fd_set             c_rfd;

    struct sockaddr_storage her_addr;
    socklen_t her_addr_size;
    struct addrinfo con_kind,
                            *addr_info_res ;
    int               random_arr[ARR_SIZE];
    int               rand_num;
    int               count_numbers_received=0;
    int               location;
    time_t            time_before,time_after,total_time;
    //int               client_counter=0;
    int               i;


    build_array(random_arr);

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

    rc = listen(main_socket, NUM_OF_CLIENTS); //listen to clients
    if (rc) {
        perror("listen failed");
        exit(EXIT_FAILURE) ;
    }
    //size_cliet_address = sizeof(cliet_address);

    her_addr_size = sizeof(her_addr);

    FD_ZERO(&rfd);
    FD_SET(main_socket, &rfd);
/*
while(client_counter<NUM_OF_CLIENTS){
	rc=read(fd,&rand_num,sizeof(int));
	if(rc>0)
		client_counter++;
}

int i;
for(i=0 ; i < NUM_OF_CLIENTS ; i++)
	write(fd,&YES,sizeof(int));
*/
//sleep(3);
    time_before=time(NULL);
    while (nums_deleted<ARR_SIZE) {
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
        		rc=read(fd,&rand_num,sizeof(int));
        		if(rc==0){
        			close(fd);
        			FD_CLR(fd,&rfd);
        		}
        		else if(rc>0)
        		{
        			count_numbers_received++;
        			if((location=find_location(random_arr,rand_num))!=NOT_EXIST){ //rand_num is exist
        				write(fd,&YES,sizeof(int));
        				delete_number(random_arr,location);
        				nums_deleted++;
        			}
        			else
        				write(fd,&NO,sizeof(int));
        		}
        		else {
        			perror("read() failes");
        			exit(EXIT_FAILURE);
        		}
        	}
    }
for(i=0; i<NUM_OF_CLIENTS; i++){
    write(fd,&FINISH,sizeof(int));
}
    time_after=time(NULL);
    total_time=time_after-time_before;
    sleep(1);
    printf("%d %d %d\n",(int)total_time,count_numbers_received,nums_deleted);
    return(EXIT_SUCCESS) ;
}
//=========================================================================
//this function terminate the program if there is an error in fork
void terminate(char *error_message){
    perror (error_message);
    exit(EXIT_FAILURE);
}
//-------------------------------------------------------------------------
void build_array(int random_arr[]){
    int i, rand_num;
    for (i = 0; i < ARR_SIZE; i++) {
        rand_num = rand() % RAND_RANGE;
        random_arr[i] = rand_num;
    }
}
//-------------------------------------------------------------------------
int find_location(int arr[],int num){
    int i;
    for(i = 0; i < ARR_SIZE ; i++){
        if(arr[i] == num)
            return i;
    }
    return NOT_EXIST;
}
//-------------------------------------------------------------------------
void delete_number(int* arr, int location){

    arr[location] = -1;
}
//-------------------------------------------------------------------------
