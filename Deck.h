#pragma once
#include<iostream>
#include<vector>
#include<stdlib.h>
#include<time.h>

using namespace std;

/*
deck class that contains deck used to play and 
*/

class Deck
{
public:
	/*create the deck*/
	void init_deck();

	/*shuffle the deck randomly*/
	void shuffler();

	/*print the order of the deck*/
	void print_deck();

	/*get a card out of the deck*/
	int  getCard();

	/*check if deck is empty*/
	bool is_deck_empty();
};

