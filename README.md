# TCP-SERVER-CLIENT
### A simple and lightweight , zero dependency C++ application to emulate a TCP server-client model that uses sockets . 

[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://travis-ci.org/joemccann/dillinger)

This Project is developed as a part of Assignment 1 of Internect Technology course offered by Jadavpur University .

- It Uses **Sockets** 
- **Multithreading** is also used to support multiple client 

## Features

- Multiple Client Supported with help of multithreading 
- authentication 
- written in C++17 sysntax
- A report is also attached [above](https://github.com/susanka068/JU_BCSE_6th_Sem/blob/master/TCP_SERVER_CLIENT/Susanka%20Majumder%20IT%20assignment%20-1%20.pdf) 


## Installation

clone this repository : 

```sh
git clone https://github.com/susanka068/TCP-server-client.git
```

## Build Instruction 

for this it is already assummed that you a standard c++ compiler installed on your machine . if not [check this out](https://youtu.be/sXW2VLrQ3Bs) or any other standard tutorial in installing a c++ compiler

build the server :

```sh
g++ -pthread server.cpp -o server
```

build the client

```sh
g++ -pthread client.cpp -o client
```

## Execution Instruction

staring the server :

```sh
./server
```

starting a client

```sh
./client <client_id>
```
<client_id> is can be given any <= 1000 

## demo

![demo screenshot](https://github.com/susanka068/TCP-server-client/blob/master/Screenshot%20from%202021-03-06%2020-34-41.png?raw=true)

**Free Software, Hell Yeah!**

**!!This is a lightwweight app to demonstrate the function of sockets in C++ and is not meant  to be used in production**

## License

MIT

[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)

  
