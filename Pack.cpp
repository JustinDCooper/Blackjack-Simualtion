#include "iostream"
#include "Pack.h"
#include <algorithm>
#include <chrono>

using namespace std;
deque<Card> standardDeck;
string suits[4] = { "Hearts","Diamonds","Clubs","Spades" };
string names[13] = { "High_Ace","Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten","Jack","Queen","King" };
int values[13] = { 11,2,3,4,5,6,7,8,9,10,10,10,10 };

int high_Low_Counts[11] = { -1,1,1,1,1,1,0,0,0,-1,-1 }; //Index position represents value of card - 1

//Constructor//
Pack::Pack(int numPacks, string state, string countStyle)
{
	srand(time(0));
	packSize = numPacks;
	countSystem = countStyle;

	if (state == "full" || state == "Full")
	{
		for (numPacks; numPacks > 0; numPacks--)
		{
			for (string s : suits)
			{
				for (int i = 0; i < 13; i++)
				{
					Card filler = { names[i],s,values[i] };
					pile.push_back(filler);
				}
			}
		}
	}
	standardDeck = pile;
}


//Primary Method Definitions//
void Pack::shuffle()
{
	random_shuffle(pile.begin(), pile.end());
}

Card Pack::draw()
{
	Card pull = pile.front();
	pile.pop_front();
	if (countSystem == "High_Low")
	{
		runningCount += high_Low_Counts[pull.value - 1];
	}
	return pull;
}

void Pack::discard(Card c)
{
	discardPile.push_front(c);
}

void Pack::reset()
{
	pile = standardDeck;	//need standard deck so low_aces automaticall raise
	shuffle();	//shuffle function is specific to playing pile
	discardPile.clear();
	runningCount = 0;
	iterations++;
	cout << iterations << " Deck reshuffles" << endl;
}


//Secondary Method Definistions//
float Pack::getTrueCount()
{
	return (float(runningCount) / (float(round(2 * float(pile.size()) / 52)) / 2));
}

float Pack::playSize()
{
	return float(pile.size());
}

bool Pack::likeleyBust(int handVal)
{
	int totalBust = 0;;
	for (Card c : pile)
	{
		if (c.name == "High_Ace" && handVal != 21)
		{
			continue;
		}
		else if (c.value + handVal > 21)
		{
			totalBust += 1;
		}
	}
	return 2*totalBust > int(pile.size());
}


//Trouble Shooting Method Definitions//
void Pack::display(bool isDiscard)
{
	if (!isDiscard)
	{
		for (Card c : pile)
		{
			cout << c.name << " of " << c.suit << endl;
		}
		return;
	}
	else
	{
		for (Card c : discardPile)
		{
			cout << c.name << " of " << c.suit << endl;
		}
		return;
	}
}

bool Pack::isEmpty()
{
	return pile.empty();
}