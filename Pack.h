#pragma once
#include <iostream>
#include <deque>
using namespace std;

struct Card
{
	string name;
	string suit;
	int value;
};

class Pack
{
	deque<Card> pile, discardPile;
	string countSystem;
public:
	int packSize;
	int iterations = 0;
	int runningCount = 0;

	Pack(int numPacks = 1,string state = "full", string method = "None");

	//Primary Methods
	void shuffle();
	Card draw();
	void discard(Card);
	void reset();

	//Secondary Methods
	float getTrueCount();
	float playSize();
	bool likeleyBust(int);

	//Troubleshooting Methods
	void display(bool isDiscard = false);
	bool isEmpty();
};