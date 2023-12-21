#pragma once
#include "Pack.h"
#include "Hand.h"
#include <vector>
using namespace std;

class Player
{
	float gain = 0;
	int bettingUnit;
	string strategy;
	string name;
	Pack* playDeck;

	//Strategic play methods
	void standardPlay();
	void omnipotentPlay();
	void basicStrategicPlay(Card,bool);
public:
	bool movesDisplayed;
	bool surrendered = false;
	Hand hand, splitHand;
	Player(string playerName, Pack* deckAdress, string method, int betSize = 0, bool displayMoves = false);

	//Primary Methods
	void placeBet(string countSystem = "None");
	void surrender();
	void hit(int iterations = 1,bool isSplitHand = false);
	void split();
	void doubleDown(bool isSplitHand = false);
	void play(Card);
	void resolve(string handWon,string splitWon = "Lost");
	void clearHands();
	void saveData(string type, float matchGain = NULL, bool handWon = false, bool splitWon = false);


	//Troubleshooting Methods
	void forcedHit(Card forcedCard, int iterations = 1);
	void printGain();
	void printHands();
};