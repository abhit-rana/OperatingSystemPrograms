Some convention:
sender of strings <=> p1 program or program who sends the strings
reciever of strings <=> p2 program or program who recieves the strings


1. Commonality in all three programs:

1.1 Sender of the strings:
the way the strings are generated, stored, fetched to send to other process and also recieve back acknowledgement from other process.

1.1.1 strings generation:
size of the string generated is 4. a random no is generated with the help of the rand() function along with srand() function(for setting seed for number generation with argument as NULL), between a fixed interval in which each integer corrosponds to the index of the array storing upper and lower case alphabets. this number generation is repeated 4 times to get 4 character corrosponding to string of size 4.

1.1.2 strings storage:
the string(of size 4) generated is stored(each of the 4 characters of string are copied) in the char array of size 250 with number of the genrated string appended at the end of the string by converting it to char. in this way 5 indices are occupied by one string and this process is repeated for 50 times filling up the 250 indices of the array thus making a array storing 50 strings. we can visualize it as blocks of size 5 with string occupying first 4 position and last one it's number in the 50 strings.

1.1.3 fetching of strings through array:
the 5 strings are fetched which implies that the 25 characters of the array storing the strings are copied to a buffer which will be used for sending to the other process. this process is repeated for 10 times equivalent to sending 50 strings.

1.1.4 recieving the acknowledgement:
the sender recieves the acknowledgement from the reciever by loading the number send in an integer and then printing that number sent.

1.2 Reciever of strings:

1.2.1 recieving the strings:
the 25 characters send by the sender are recieved in a buffer of size 25 and then loop operates on these 25 characters starting from 0 and we print 4 characters starting from zero(which forms the string sent by the sender) and on the 5th character we did not print it as it is the index of the string and this process is repeated for the other 4 blocks of size 5 corrosponding to the remaining 20 characters.

1.2.2 sending the acknowledgement:
we make a integer variable initialized to -1 and then while we were traversing through the sent 25 characters we leaved every index which is a multiple of 5 and at that time we comapre the integer variable with these values at indices of multiples of 5 and update it if it is less than the index of the string and when the loop ends we get the index of string which is maximum and then we sent it to the sender process of strings.

2. Different things in the three programs:
the way communication occurs between these processes is different in these three programs

Using Fifo:
the two fifo files are generated with the mkfifo() with the permission to read and write and name of the file to be kept. the generated files are then opened by the open() system call and the file descriptor sent back is stored in a variable. In both of these files the two process uses it to write or read the data by read() and write() system calls with one of the argument as the file descriptor. in this case we also have to use the sleep() for the process which recieves strings as it ends as soon it recieves last 5 strings and sending the acknowledgement making sender of strings unable to read the the acknowledgement due to blocking property of the fifo as one of the ends of the fifo closes.

Using Sockets:
we implemented the unix domain sockets and gave the name for the server and the client file. the sender of the strings binded as a server to the socket and reciever of strings binded as client to the socket. we used socket() system call with indicated type as SOCK_SEAQPACKET for transferring data in the form of preserved boundaries and also store the returned value in int varible and also listened to one client via listen() system call. we filled sockaddr_un structure for both p1 and p2 programs to make them ready to bind to sockets. we also used accept()(to accept the client requests) and connect()(to connect to the server). the server and client communicate with each other 10 times where server sending 5 strings at a time via send() and rval() for sending and recieving the information respectively with one of the argument as the identifier for the socket creaated. we close the socket created by the close() system call by passing the identifier for the socket as the argument.

Using Message Queues:
two message queues are created by the msgget() function with return as integer and stored in an integer variable and also permission set to read and write by all possible members(user, group and other). we also created two different structures with common as message type i.e. the way message to sent and other corrosponding to our needs at one time we use an array of size 26(for sending the strings) Bytes and other time of size 2(for sending the acknowledgement) bytes. the sender and reciever both uses msgsnd() and msgrcv() function to send and recieve the message respectively with argument as the identifier for the queue and the data to be sent or recieved. we delete the message queue throug msgcntl() function by passsing the queue identifier as one of the argument.
