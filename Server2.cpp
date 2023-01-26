#include <iostream> //cout
#include <netdb.h> //addrinfo
#include <string.h> //memset
#include <sys/time.h> //gettimeofday
#include <unistd.h>	//sockets
#include <map>
#include "dealer.h"


/*
Server program that setups a server that will have connections made to it. Each new connection will create a new thread. The speed of the connections
will be out put*/

using namespace std;
int PORT;
int repitition;
int serverRun;
Dealer* dealer;
int command;

struct thread_data {
	int threadCount;
	int fileDescriptor;
};


/**
*takes in an array of command line arguments and checks each command
*for validity
*@param args
*@return -1 if input was invalid, 0 if input is valid
*/
int assignInput(char* args[]) {
	try {
		PORT = stoi(args[1]);

	}
	catch (invalid_argument & exception) {
		cout << "Wrong input, enter integers only" << endl;
		cout << exception.what();
		return -1;
	}
	catch (out_of_range & exception) {
		cout << "Wrong range of input, enter within integer range" << endl;
		cout << exception.what();
		return -1;
	}

	if (PORT < 1024 || PORT > 65535) { //max port of 65535 
		cout << "Port entered not within range" << endl;
		return -1;
	}

	return 0;
}

/*
gets username input to display into the server cout*/
void scoreBoard(int sock) {
	char buffer[200];
	read(sock, (char *)buffer, 200);
	printf("User Joined: %s\n", buffer);

}

/**
*function that needs to be created for multi threading.
*@param threadData data that will be used by the function and supplied by pthread
*/
void* threadProcess(void* threadData) {
	struct thread_data* data;
	data = (struct thread_data*) threadData;

	//while(true)
	dealer->start_game(data->fileDescriptor);
	close(data->fileDescriptor);
	data->threadCount--;
}

/*
seperate command that takes in server commands*/
void* serverCmds(void*) {

	char cmd;
	while (true) {
		cin >> cmd;
		if (cmd == 'E') {//close the server, done after games are complete
			delete dealer;
			exit(0);
		}

	}
}



/**
*Main function that will setup a socket and listen for clients to connect to. After a connection is made
*a new thread will be made for the program to run on
*@param argc number of arguments
*@param argv array of arguments
*return -1 if connection not able to be setup, 0 if function finishes
*/
int main(int argc, char* argv[]) {
		//check command line characters for validity
	if (argc != 2) {
		cout << "invalid argument amount" << endl;
		return -1;
	}
	if (assignInput(argv) == -1) {
		cout << "invalid arguments" << endl;
		return -1;
	}


	pthread_t closeThread;
	pthread_create(&closeThread, nullptr, serverCmds, nullptr);//start input thread


	//setup addrinfo
	int status;
	struct addrinfo hints;
	struct addrinfo* servinfo;  // will point to the results

	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(nullptr, argv[1], &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return -1;
	}

	struct addrinfo* connectionsFound;
	int socketFileDesc;
	int bindStatus;
	//find correct connection
	for (connectionsFound = servinfo; connectionsFound != nullptr; connectionsFound = connectionsFound->ai_next) {
		
		//setup socket
		socketFileDesc = socket(connectionsFound->ai_family, connectionsFound->ai_socktype, connectionsFound->ai_protocol);
		if (socketFileDesc == -1) {
			cout << "invalid socket found " << endl;
			continue;
		}
		//connect to socket
		bindStatus = bind(socketFileDesc, connectionsFound->ai_addr, connectionsFound->ai_addrlen);
		if (bindStatus == -1) {
			cout << "invalid bind created" << endl;
			continue;
		}

		cout << "successful bound" << endl;
		break;
	}

	//no connection found
	if (connectionsFound == nullptr) {
		fprintf(stderr, "client: failed to connect\n");
		return -1;
	}

	freeaddrinfo(servinfo);
	//listen for data
	int listenUsingSocketResult = listen(socketFileDesc, 10);
	if (listenUsingSocketResult != 0)
	{
		cout << "Unable to listen" << endl;
		return -1;
	}
	int threadCount = 0;

	dealer = new Dealer();//single dealer and deck is used for all connections

	while (true) {//continously get new users until server full

		struct sockaddr_storage their_addr;
		socklen_t addr_size;
		int newFileDesc;
		addr_size = sizeof their_addr;
		newFileDesc = accept(socketFileDesc, (struct sockaddr *)&their_addr, &addr_size );
		if (newFileDesc == -1) {
			cout << "unable to connect to client" << endl;
			continue;
		}

		thread_data data;
		pthread_t thread;
		int threadStatus;

		data.threadCount = threadCount;
		data.fileDescriptor = newFileDesc;
		scoreBoard(newFileDesc);

		//thread creation
		threadStatus = pthread_create(&thread, nullptr, threadProcess, (void*)&data);//start game thread
		if (threadStatus != 0) {
			cout << "new thread not created" << endl;
			continue;
		}
		threadCount++;

	}
}