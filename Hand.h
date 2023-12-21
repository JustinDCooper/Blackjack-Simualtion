#pragma once
#include <vector>
#include "Pack.h"	//for Card type

using namespace std;
class Hand
{
public:
	vector<Card> heldCards;
	int bet = 0;

	//Primary Methods//
	int getVal();
	bool isBust();
	bool isBlackJack();
	bool isPair();
	bool isSoft17();
	bool containsLow_Ace();
	bool containsHigh_Ace();
	void raiseLow_Ace();
	void lowerHigh_Ace();
	void discardHand(Pack*);
};