# Final_Project
This Repo is created for the Systems II final project
@Project Name: Keylogger
@Authors: Babak Rafian, Greg Ochs

Instructions: 

After compiling client.c, server.c and KeyLogger.c runder server first with providing a port number (any available port number on the machine) as follow:
>> ./server 35454 

Then run the client with providing the server's ip address and port number as follow:
>> ./client server-ip 35454

This should start the keylogger which writes every keypress to a text file called log and client will send the content of the file to the server.
