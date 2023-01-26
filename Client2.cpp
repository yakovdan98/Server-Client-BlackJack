#include <iostream> //cout
#include <netdb.h> //addrinfo
#include <string.h> //memset
#include <sys/time.h> //gettimeofday
#include <unistd.h>	//sockets
#include "Player.h"

/*By Daniel Yakovlev 10/26/20
Client program that connects to a server using a port, ip address and send data based on a total buffer size of 1500
multiple repititions can be used to resend data, the program will time the operation and determine how long it takes*/

using namespace std;


const int PORT = 4198;
const int BUFFSIZE = 200;


char* serverIP;
char* username;


/**
*takes in an array of command line arguments and checks each command
*for validity
*@param args
*@return -1 if input was invalid, 0 if input is valid
*/
int assignInput(char* args[]) {

	serverIP = args[1];

	return 0;

}

/*
black jack game
@param sock socket to use for messages
*/
void blackJackGame(int sock) {
	char buffer[BUFFSIZE];
	int inVal;
	int dealerSum = 0;
	std::string inType;

	Player* player = new Player(sock);

	read(sock, buffer, BUFFSIZE);
	inVal = buffer[0];
	dealerSum += inVal;
	cout << endl;
	std::cout << "Dealers First Card: " << inVal << " " << std::endl;
	cout << endl;

	for (int i = 0; i < 2; i++) {//get two cards for player
		read(sock, buffer, BUFFSIZE);
		inVal = buffer[0];
		player->addCard(inVal, inType);
	}
	std::cout << "Your Cards " << std::endl;
	player->printCards();

	while (player->getClientTurn()) {//keep getting cards until player busts or stands
		cout << endl;
		player->playerChoice();
		if (player->getBusted()) {
			std::cout << "player busted!" << std::endl;
			break;
		}
	}


	while (dealerSum < 17) {//dealer keeps getting cards
		cout << endl;
		read(sock, buffer, BUFFSIZE);
		int in = buffer[0];
		dealerSum += in;
		std::cout << "dealer drew " << in << std::endl;
		std::cout << "dealer has " << dealerSum << std::endl;
	}

	//std::cout << dealerSum << std::endl;
	//std::cout << player->getSum() << std::endl;
	//std::cout << player->getBusted() << std::endl;
	cout << endl;
	if (dealerSum > player->getSum() && dealerSum <= 21) {
		std::cout << "Dealer Won!" << std::endl;

	}
	else if (dealerSum > 21 && player->getBusted()) {
		std::cout << "Push!" << std::endl;

	}
	else if (player->getBusted()) {
		std::cout << "Dealer Won!" << std::endl;


	}
	else if (dealerSum == player->getSum()) {
		std::cout << "Push!" << std::endl;


	}
	else {
		std::cout << "You Have Won!" << std::endl;

	}




	delete player;
	player = nullptr;

}

/**
*connect function that will setup a socket and connect to a server, write using 3 different ways
*and then wait for a response, the times to send, and recieve a response will ne take and output
*@param argc number of arguments
*@param argv array of arguments
*return -1 if connection not able to be setup, 0 if function finishes
*/
int connect(int argc, char* argv[]) {
	//check command line characters for validity
	if (argc != 2) {
		cout << "invalid argument amount" << endl;
		return -1;
	}
	if (assignInput(argv) == -1) {
		cout << "invalid arguments" << endl;
		return -1;
	}

	//setup addrinfo
	int status;
	struct addrinfo hints;
	struct addrinfo* servinfo;  // will point to the results

	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

	char port[16];
	snprintf(port, sizeof(port), "%ld", PORT);
	status = getaddrinfo(serverIP, port, &hints, &servinfo);


	// get ready to connect
	if (status != 0) {

		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return -1;
	}

	struct addrinfo* connectionsFound;
	int socketFileDesc;
	int connectionStatus;


	//find correct connection
	for (connectionsFound = servinfo; connectionsFound != nullptr; connectionsFound = connectionsFound->ai_next) {

		//setup socket
		socketFileDesc = socket(connectionsFound->ai_family, connectionsFound->ai_socktype, connectionsFound->ai_protocol);
		if (socketFileDesc == -1) {
			cout << "invalid socket found " << endl;
			continue;
		}
		//connect to socket
		connectionStatus = connect(socketFileDesc, connectionsFound->ai_addr, connectionsFound->ai_addrlen);
		if (connectionStatus == -1) {
			cout << "invalid connection created" << endl;
			continue;
		}

		//cout << "connected " << endl;
		break;
	}

	//no connection found
	if (connectionsFound == nullptr) {
		fprintf(stderr, "client: failed to connect\n");
		return -1;
	}

	freeaddrinfo(servinfo); // all done with this structure



	return socketFileDesc;
}

/*
main function, connect and game functions called. Checks if user wants to play another game
@param argc num of arguments
@param argv[] argument array
return status of program
*/
int main(int argc, char* argv[]) {

	cerr << "Enter your Username" << endl;

	char in = 'Y';

	char message[200];
	cin >> message;


	while (in == 'Y') {
		system("clear");

		int sock = connect(argc, argv);//form connection
		write(sock, (char*)message, BUFFSIZE);//write username of user
		blackJackGame(sock);//begin game
		close(sock);//end game

		std::cout << "enter Y to play again N to quit" << std::endl;//check for another game

		while (true) {
			std::cin >> in;
			if (in == 'Y')
				break;
			else if (in == 'N')
				return 0;
			else
				std::cout << "invalid command: enter Y/N" << std::endl;

		}
	}



	return 0;
}