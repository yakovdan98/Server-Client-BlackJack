#pragma once
#include <vector>
#include <cstring>
#include <unistd.h>
#include <iostream>

/*
player class that has information on player, gets status on user whenever a card is pulled for user
*/

class Player
{
	struct Card {
		int value;
		std::string type;
	};

private:
	bool busted;//> 21
	bool clientTurn;//standing
	int cardSum;
	int sock;
	std::vector<Card> *cards;

public:
	/*
	constructor
	@param sock socket to read and write to*/
	Player(int sock);

	/*
	destructor*/
	~Player();
	
	/*
	get another card*/
	void hit();
	
	/*
	stand, dont pull card*/
	void stand();
	
	/*
	have player decide what to do next*/
	void playerChoice();
	
	/*
	get the value the cards add up to
	return value of cards*/
	int getSum();

	/*
	get client turn status, see if client can pget another card
	return true if they can, false if they can't*/
	bool getClientTurn();
	
	/*
	see if client has gone over total card value of 21
	return true over 21, false under 21*/
	bool getBusted();
	
	/*
	reset client information*/
	void reset();

	/*
	give another card to player*/
	void addCard(int val, std::string type);
	
	/*
	print all the cards that the player has*/
	void printCards();
	
};

