# UDP-Server

My Goal: to please you!

Lior Sapir - liorsap1@gmail.com

UDP sockets with Select
In this exercise I’ll implement a simple echo server using UDP and select().

I’ll supply the client program in other repository. The client reads a message from the command line, send the message to the server, read a reply message form the server and print it to the screen.
I run the client like this:
./client <server-host-name> <port>
When the client reads a message from the client, it reads it till a new line appears.

The server run like this:
./server <port>
The server can talk with many clients but on a single socket.
The server gets a message from the client, change it to upper case and returns it back to the specific client. 
Also here, there is only one thread, therefore, any I/O operation never block.
I handle the maximum length of a message is 4096.
I use select with a single fd set both in the readset and the writeset. Then before I/O operations I verify that the socket is ready for the operation.
I maintain a queue to save read messages and the source information. Only when the queue in not empty I check if the socket is ready to write. I always check if the socket is ready for reading operation. 
In case of any failure, I use perror/fprintf and exit.
Whenever my socket is ready to read or write I print:
“server is ready to read\n”  or
“Server is ready to write\n”
my program never get in “busy-wait”

GOOD-LUCK!
Wish you the best!!!
