#include "dealer.h"

using namespace std;

const int BUFFSIZE = 200;

Dealer::Dealer()
{
	sum = 0;
	deck = new Deck();
	deck->init_deck();
	deck->shuffler();
	deck->print_deck();
}

Dealer::~Dealer()
{
	delete deck;
	deck = nullptr;
}


void Dealer::start_game(int sock) {
	char buffer[BUFFSIZE];

		for (int i = 0; i < 3; i++) {//get first 3 cards to player, 1 dealer card, 2 player cards

			int cardVal = deck->getCard();
			sum += cardVal;
			buffer[0] = cardVal;
			write(sock, (char*)buffer, BUFFSIZE);
			cout << cardVal << endl;
		}

		while (true) {//keep recieving input from player until player stands or busts
			read(sock, (char*)buffer, BUFFSIZE);
			cout << buffer[0] << endl;
			if (buffer[0] == 'S' || buffer[0] == 'B') {
				break;
			}
			if (buffer[0] == 'H') {
				buffer[0] = deck->getCard();
				write(sock, (char*)buffer, BUFFSIZE);
			}

		}

		while (sum < 17) {//keeping puling cards until dealer reaches > 17
			int cardVal = deck->getCard();
			sum += cardVal;
			buffer[0] = cardVal;
			write(sock, (char*)buffer, BUFFSIZE);

		}
		sum = 0;




	//}

	//Close each connection -- Both Client and Server Connection -- 
}
