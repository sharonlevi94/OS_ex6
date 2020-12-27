# OS_ex6
// OS- semester A- class B2 
// Bar Ifrah-  login:- barif 
// Sharon Levi  login:- sharonlevi 
// Date - December 27th, 2020
**********************************************************************************
ALL PROGRAMS ARE EXECUTED VIA UNIX TERMINAL ALL PROGRAMS COMPILED VIA TERMINAL-
USING COMMAND "gcc -Wall (filename.cc) -o (runfilename) 
Programs are executed via terminal. './<runFileName>.
********************************************************************************** 
//ex5a
  //ex5a1: server
// this program creates the random numbers array and the socket.
// after checking that we have 3 different number makers, we look for
// numbers to delete. when receive a number, it checks if the number exists in the array.
// if so- it will bw deleted and replaced with '-1'. if not, continue;
// when the array is all set to be -1, we finish .
// the output will be- how much time it took.

  //ex5a2: client - number maker
// this program is the number creator for ex5a1
// here we create random numbers. the numbers are moved to ex5a1 via socket.
// output will be- numbers sent, number deleted.
  // race modes-
// none that we are aware of.
// output will be- 3 subprocesses data(as described), master time.
**********************************************************************************
//ex6b
   //ex6b1 - gcd server
 //this program is a server that create a socket and wait for 2 input numbers
 //from clients. while it get a numbers, it caclulate their gcd and send the 
 //answer to the client. the program will finish by ctrl+c in the terminal.
 //execute line (in terminal) : ./ex6b1 <PORT NUMBER1>
 //input & output - None.
 
   //ex6b2 - prime factors server
 //this program is a server that create a socket & wait for number from clients.
 //when it read a number from the socket, it find all the prime factors of the
 //number it got, then it sent all the factors to the client.
 // the program will finish by ctrl+c in the terminal.
 //execute line (in terminal) : ./ex6b1 <PORT NUMBER2>
 //input & output - None.
 
   //ex6b3 - client
 //this program is a client that connect to 2 sockets of 2 servers: gcd server &
 //prime factor server. the user choose between 2 actions and input a char:
 //g for gcd action or d for prime factors action. the client send the input via 
 //the socket to the correct server and print the result the it got from the server.
 //the program will finish by ctrl+c in the terminal.
 //input: 'g' and then two numbers.
 //input: 'd' and then one number.
 //execute line (in terminal) :
 //./ex6b1 <IP ADDRESS> <PORT NUMBER1>  <IP ADDRESS> <PORT NUMBER2>
 

