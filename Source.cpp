#include "Pack.h"
#include "Player.h"
#include "Hand.h"
using namespace std;
struct playStyle
{
	string gamblerStrategy, countSystem;
}
Standard = {"Standard","None"}, 
Omnipotent = {"Omnipotent", "None"}, 
Dealer{"Dealer", "None"}, 
Basic = {"Basic", "None"}, 
High_Low = {"Count", "High_Low"};
//Assign Values Here///////////////////////////////////////////////////////////
int packCount = 6; //how many packs are being played
int maxGames = 100; //how many times will the deck be played
int betUnit = 20;
playStyle strat = Basic;

bool display = true;
///////////////      House Rules in Player.cpp      ///////////////////////////

string playerWon(Hand dealerHand, Hand gamblerHand)
{
	if (gamblerHand.heldCards.empty())
	{
		return "tie";
	}
	else if (gamblerHand.isBust())
	{
		return "lost";
	}
	else if (dealerHand.isBust())
	{
		return "won";
	}
	else if (gamblerHand.getVal() > dealerHand.getVal())
	{
		return "won";
	}
	else if (gamblerHand.getVal() < dealerHand.getVal())
	{
		return "lost";
	}
	else if (gamblerHand.getVal() == dealerHand.getVal())
	{
		return "tie";
	}
	return "tie";
} 
//Winning hand returns true

void playRound(Player* dealer, Player* gambler)
{
	gambler->placeBet(strat.countSystem); //Place bet in accordance with method

	dealer->hit(2);
	gambler->hit(2);

	//If dealer gets Blackjack, gambler loses automatically
	if (dealer->hand.isBlackJack())
	{
		if (gambler->movesDisplayed)
		{
			cout << "Dealer Blackjack\n";
		}
		gambler->resolve("lost");
		dealer->clearHands();
		return;
	}
	else
	{
		//Play hands
		gambler->play(dealer->hand.heldCards.front());
		dealer->play(dealer->hand.heldCards.front());

		//Print moves if told
		if (gambler->movesDisplayed)
		{
			dealer->printHands();
			gambler->printHands();
		}

		string handResult = playerWon(dealer->hand, gambler->hand), 
			 splitResult = playerWon(dealer->hand, gambler->splitHand);

		//Settle bets and reset hands
		gambler->resolve(handResult, splitResult);

		//Discard dealer Hands
		dealer->clearHands();
	}
}

int main()
{
	Pack deck(packCount, "Full", strat.countSystem);
	deck.shuffle();
	
	Player dealer("Dealer" ,&deck, "Dealer"), gambler("Gambler", &deck, strat.gamblerStrategy, betUnit, display);

	while (deck.iterations < maxGames)
	{
		playRound(&dealer, &gambler);
	}
	return 0;
}