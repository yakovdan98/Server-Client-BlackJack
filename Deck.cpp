#include "Deck.h"


int Ace = 1;
int J = 10;
int Q = 10;
int K = 10;
vector<int>deck;

void Deck::init_deck() {//initial order of deck
	deck = { Ace,2,3,4,5,6,7,8,9,10,J,Q,K,
			 Ace,2,3,4,5,6,7,8,9,10,J,Q,K,
			 Ace,2,3,4,5,6,7,8,9,10,J,Q,K,
			 Ace,2,3,4,5,6,7,8,9,10,J,Q,K };
}

void Deck::shuffler() {//shuffle deck based on srand time
	srand((unsigned)time(0));
	for (int i = 0; i < 52; i++)
		swap(deck[i], deck[rand() % 52]);//swap card at i with random card
	cout << "******Deck Shuffled!******" << endl;
}

void Deck::print_deck() {
	cout << "deck:" << endl;
	int t = 0;
	for (auto i : deck) {
		cout << i << " ,";
		t++;
		if (t % 13 == 0) cout << endl;//end line after every 13 cards
	}
}

int Deck::getCard() {
	if (is_deck_empty()) {//check if deck empty before pulling card
		init_deck();//if deck empty create a new deck and shuffle it
		shuffler();
	}
	int card = deck.front();
	deck.erase(deck.begin());//remove card
	return card;
}

bool Deck::is_deck_empty() {
	return deck.empty();
}