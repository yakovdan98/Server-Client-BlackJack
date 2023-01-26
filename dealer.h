#pragma once
#include <cstring>
#include <unistd.h>
#include <iostream>
#include "Deck.h"

/* dealer class that will run the main game and send cards to players*/

class Dealer
{
public:
	/*
	constructor*/
	Dealer();

	/*
	destructor*/
	~Dealer();

	/*
	main game function for server*/
	void start_game(int sock);

private:
	int sum;
	Deck* deck;//deck to pull cards from
};

