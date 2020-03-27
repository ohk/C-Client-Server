# C Client - Server for execute Shell Commands
This server-client software enables to perform "shell commands" entered by the client on the server. 

# Usage
```
./UKSU_server -r <portNumber> -u <username> -p <password>
./UKSU_client -h <host> -r <port> -u <username> -p <password>
```
First of all, the server needs to stand up. Then you can connect the client. 
The username and passwords you provided when running both the server and the client must be the same.

# TODO

[] Cannot verify if username and password are long
[] Shell commands is processing but giving an error message.