# ECE312Projects


Compile the Client and Server
gcc ChatClient.c -o client -pthread
gcc ChatServer.c -o server -pthread

Running them (Run server first)
./server
./client hostname

ex for if on same machine: 
./server
./client localhost

They use port 4567 as default, you can change the port in the source or 
add in an arguement check from the command line. Either way is fine if you
want to change it.
