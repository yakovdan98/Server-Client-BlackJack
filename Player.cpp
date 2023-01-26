#include "Player.h"


const int BUFFSIZE = 200;

char buffer[BUFFSIZE];


Player::Player(int sock)
{
	this->sock = sock;
	clientTurn = true;
	busted = false;
	cardSum = 0;
	cards = new std::vector<Card>();
}

Player::~Player()
{
	delete cards;
}

void Player::hit()
{
	//send H
	buffer[0] = 'H';
	write(sock, (char *)buffer, BUFFSIZE);//send request for another card
	std::cout << "player hits" << std::endl;
	std::cout << std::endl;

	read(sock, buffer, BUFFSIZE);//read card recieved
	int inVal = buffer[0];
	
	addCard(inVal, "regular"); 
	printCards();
}

void Player::stand()
{
	//send S
	buffer[0] = 'S';
	write(sock, (char*)buffer, BUFFSIZE);//send stand response, no more cards
	clientTurn = false;
	std::cout << "player stands" << std::endl;
	std::cout << std::endl;

}

void Player::playerChoice()
{
	char input;
	while (true) {
		std::cout << "Enter H to Hit, S to Stand" << std::endl;
		std::cin >> input;
		if (input == 'H') {
			hit();
			break;
		}
		else if (input == 'S') {
			stand();
			break;
		}
		else {
			std::cout << "Invalid input, Enter H or S" << std::endl;

		}
	}
}

int Player::getSum()
{
	int retVal = 0;
	for (int i = 0; i < cards->size(); i++) {
		retVal += cards->at(i).value;
	}
	if (retVal > 21) {
		//write out B
		buffer[0] = 'B';
		write(sock, (char*)buffer, BUFFSIZE);
		busted = true;
		clientTurn = false;
	}
	return retVal;
}

bool Player::getClientTurn()
{
	return clientTurn;
}

bool Player::getBusted()
{
	return busted;
}

void Player::reset()
{
	cards->clear();
	cardSum = 0;
	clientTurn = true;
	busted = false;
}

void Player::addCard(int val, std::string type)
{
	Card card;
	card.value = val;
	card.type = type;
	cards->push_back(card);
}

void Player::printCards()
{
	std::cout << "card sum " << getSum() << std::endl;
	for (int i = 0; i < cards->size(); i++) {
		std::cout << "Card " << i << std::endl;
		std::cout << cards->at(i).value << std::endl;
	}
}


