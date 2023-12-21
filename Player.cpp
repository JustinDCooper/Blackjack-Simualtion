#include "Player.h"
#include <fstream>
#include <string>

using namespace std;
struct Rules
{
	bool hitOnSoft17;
	bool doubleAfterSplit;
	bool surrender;
	float BJPayout;
	float pen;
}house = { true,true,false, 1.5f , 0.9f};

//Concept Functions
void stand(bool movesDisplayed) //Stand has no actual function but declaring allows easy access for troubleshooting
{
	if (movesDisplayed)
	{
		cout << "Stand\n";
	}
	return;
}

//Strategic play methods
void Player::standardPlay()
{
	if (hand.getVal() == 22)//if hand is pair of aces lower one ace
	{
		hand.lowerHigh_Ace();
	}
	while (hand.getVal() < 17 || (house.hitOnSoft17 && hand.isSoft17()))
	{
		hit();
	}
	return;	//holding is equivalent to doing no more (i.e. return)
}

void Player::omnipotentPlay()
{
	if (hand.getVal() == 22)//if hand is pair of aces lower one ace
	{
		hand.lowerHigh_Ace();
	}
	while (!hand.isBust() && !playDeck->likeleyBust(hand.getVal()))
	{
		hit();
	}
}

void Player::basicStrategicPlay(Card dealerCard, bool isSplitHand = false)
{
	Hand* playingHand = isSplitHand ? &splitHand : &hand;
	int handTotal = playingHand->getVal();


	//Determine if hand should surrender if surrender allowed
	auto shouldSurrender = [=]()
	{
		switch (handTotal)
		{
		case 15: return dealerCard.value == 10;
		case 16:
			switch (dealerCard.value)
			{
			case 9: return true;
			case 10: return true;
			case 11: return true;
			default: return false;
			}
		default: return false;
		}
	};
	if (house.surrender && !isSplitHand && shouldSurrender())
	{
		surrender();
		return;
	}

	//Quickly determine if hand should stand
	auto shouldStand = [=]()
	{
		if (handTotal == 20 || handTotal == 21) //if hand is 20 or BlackJack never do anything
		{
			return true;
		}
		else if (playingHand->isPair()) //Consider split if hand is a pair
		{
			return false;
		}
		else if (playingHand->containsHigh_Ace()) //Soft Total
		{
			switch (handTotal)
			{
			case 20: return true;
			case 19: return (dealerCard.value != 6); //Stand all cases except when dealer shows a 6
			case 18: return (dealerCard.value == 7 || dealerCard.value == 8); //Stand when dealer shows 7 or 8
			default: return false;
			}
		}
		else //Hard Total
		{
			if (handTotal >= 17)
			{
				return true;
			}
			else
			{
				switch (handTotal)
				{
				case 16:
					switch (dealerCard.value)
					{
					case 2: return true;
					case 3: return true;
					case 4: return true;
					case 5: return true;
					case 6: return true;
					default: return false;
					}
				case 15:
					switch (dealerCard.value)
					{
					case 2: return true;
					case 3: return true;
					case 4: return true;
					case 5: return true;
					case 6: return true;
					default: return false;
					}
				case 14:
					switch (dealerCard.value)
					{
					case 2: return true;
					case 3: return true;
					case 4: return true;
					case 5: return true;
					case 6: return true;
					default: return false;
					}
				case 13:
					switch (dealerCard.value)
					{
					case 2: return true;
					case 3: return true;
					case 4: return true;
					case 5: return true;
					case 6: return true;
					default: return false;
					}
				case 12:
					switch (dealerCard.value)
					{
					case 4: return true;
					case 5: return true;
					case 6: return true;
					default: return false;
					}
				default: return false;
				}
			}
		}
	};
	if (shouldStand())
	{
		stand(movesDisplayed);
		return;
	}
	//Not necessary for function but should increase speed


	//Determine if hand should split based off the pair and visible dealer card
	auto shouldSplit = [=]()
	{
		switch (playingHand->heldCards[0].value) //Condition should always be the value of the "hand" card as split hand should never be considered
		{
		case 11:
			return true;
		case 9:
			switch (dealerCard.value)
			{
			case 2: return true;
			case 3: return true;
			case 4: return true;
			case 5: return true;
			case 6: return true;
			case 8: return true;	//do not split against dealers 7
			case 9: return true;
			default: return false;
			}
		case 8:
			return true;
		case 7:
			switch (dealerCard.value)
			{
			case 2: return true;
			case 3: return true;
			case 4: return true;
			case 5: return true;
			case 6: return true;
			case 7: return true;
			default: return false;
			}
		case 6:
			switch (dealerCard.value)
			{
			case 2: return house.doubleAfterSplit;
			case 3: return true;
			case 4: return true;
			case 5: return true;
			case 6: return true;
			default: return false;
			}
		case 4:
			switch (dealerCard.value)
			{
			case 5: return house.doubleAfterSplit;
			case 6: return house.doubleAfterSplit;
			default: return false;
			}
		case 3:
			switch (dealerCard.value)
			{
			case 2: return house.doubleAfterSplit;
			case 3: return house.doubleAfterSplit;
			case 4: return true;
			case 5: return true;
			case 6: return true;
			case 7: return true;
			default: return false;
			}
		case 2:
			switch (dealerCard.value)
			{
			case 2: return house.doubleAfterSplit;
			case 3: return house.doubleAfterSplit;
			case 4: return true;
			case 5: return true;
			case 6: return true;
			case 7: return true;
			default: return false;
			}
		default:
			return false;
		} 
	};
	//Split the hand if it can and should
	if (!isSplitHand && hand.isPair() && shouldSplit())
	{
		split();
		handTotal = playingHand->getVal(); //Reassign hand total value following split and hit
		basicStrategicPlay(dealerCard,true); //play the split hand
	}


	//Determine if hand should doulbledown based off the hand and visible dealer card
	auto shouldDoubleDown = [=]()
	{
		if (playingHand->containsHigh_Ace()) //Soft Totals
		{
			switch (handTotal)
			{
			case 20: return false;
			case 19: return dealerCard.value == 6 ? true : false;
			case 18: 
				switch (dealerCard.value)
				{
				case 2: return true;
				case 3: return true;
				case 4: return true;
				case 5: return true;
				case 6: return true;
				default: return false;
				}
			case 17:
				switch (dealerCard.value)
				{
				case 3: return true;
				case 4: return true;
				case 5: return true;
				case 6: return true;
				default: return false;
				}
			case 16:
				switch (dealerCard.value)
				{
				case 4: return true;
				case 5: return true;
				case 6: return true;
				default: return false;
				}
			case 15:
				switch (dealerCard.value)
				{
				case 4: return true;
				case 5: return true;
				case 6: return true;
				default: return false;
				}
			case 14:
				switch (dealerCard.value)
				{
				case 5: return true;
				case 6: return true;
				default: return false;
				}
			case 13:
				switch (dealerCard.value)
				{
				case 5: return true;
				case 6: return true;
				default: return false;
				}
			default: return false;
			}
		}
		else//Hard Totals
		{
			switch (handTotal)
			{
			case 11: return true;
			case 10:
				switch (dealerCard.value)
				{
				case 2: return true;
				case 3: return true;
				case 4: return true;
				case 5: return true;
				case 6: return true;
				case 7: return true;
				case 8: return true;
				case 9: return true;
				default: return false;
				}
			case 9:
				switch (dealerCard.value)
				{
				case 3: return true;
				case 4: return true;
				case 5: return true;
				case 6: return true;
				default: return false;
				}
			default: return false;
			}
		}
	};
	//Doubledown if it should (Can only do before any hit)
	if ((!isSplitHand || house.doubleAfterSplit) && shouldDoubleDown())
	{
		doubleDown(isSplitHand);
		return;
	}


	//Determine if should hit based off the hand and visible dealer card
	auto shouldHit = [=]()
	{
		if (playingHand->isBust()) //Do not hit if hand is bust
		{
			return false;
		}
		else if (playingHand->containsHigh_Ace()) //Soft Totals
		{
			switch (playingHand->getVal())
			{
			case 18:
				switch (dealerCard.value)
				{
				case 9: return true;
				case 10: return true;
				case 11: return true;
				default: return false;
				}
			case 17:
				switch (dealerCard.value)
				{
				case 3: return false;
				case 4: return false;
				case 5: return false;
				case 6: return false;
				default: return true;
				}
			case 16:
				switch (dealerCard.value)
				{
				case 4: return false;
				case 5: return false;
				case 6: return false;
				default: return true;
				}
			case 15:
				switch (dealerCard.value)
				{
				case 4: return false;
				case 5: return false;
				case 6: return false;
				default: return true;
				}
			case 14:
				switch (dealerCard.value)
				{
				case 5: return false;
				case 6: return false;
				default: return true;
				}
			case 13:
				switch (dealerCard.value)
				{
				case 5: return false;
				case 6: return false;
				default: return true;
				}
			default: return false;
			}
		}
		else //Hard Totals
		{
			switch (playingHand->getVal())
			{
			case 16:
				switch (dealerCard.value)
				{
				case 7: return true;
				case 8: return true;
				case 9: return true;
				case 10: return true;
				case 11: return true;
				default: return false;
				}
			case 15:
				switch (dealerCard.value)
				{
				case 7: return true;
				case 8: return true;
				case 9: return true;
				case 10: return true;
				case 11: return true;
				default: return false;
				}
			case 14:
				switch (dealerCard.value)
				{
				case 7: return true;
				case 8: return true;
				case 9: return true;
				case 10: return true;
				case 11: return true;
				default: return false;
				}
			case 13:
				switch (dealerCard.value)
				{
				case 7: return true;
				case 8: return true;
				case 9: return true;
				case 10: return true;
				case 11: return true;
				default: return false;
				}
			case 12:
				switch (dealerCard.value)
				{
				case 2: return true;
				case 3: return true;
				case 7: return true;
				case 8: return true;
				case 9: return true;
				case 10: return true;
				case 11: return true;
				default: return false;
				}
			case 10:
				return (dealerCard.value == 10 || dealerCard.value == 11);
			case 9:
				switch (dealerCard.value)
				{
				case 2: return true;
				case 7: return true;
				case 8: return true;
				case 9: return true;
				case 10: return true;
				case 11: return true;
				default: return false;
				}
			case 8: return true;
			case 7: return true;
			case 6: return true;
			case 5: return true;
			default: return false;
			}
		}
	};
	//Hit while should
	while (shouldHit())
	{
		hit(1, isSplitHand);
		if (movesDisplayed)
		{
			cout << "hit\n";
		}
	}

	//Stand when you shouldn't do anything more
	stand(movesDisplayed);
	return;
}


//Constructor
Player::Player(string playerName, Pack* deckAdress, string method, int betSize, bool displayMoves)
{
	name = playerName;
	strategy = method;
	playDeck = deckAdress;
	movesDisplayed = displayMoves;
	bettingUnit = betSize;

	//Clear Data files if not dealer
	if (playerName != "Dealer")
	{
		ofstream outFile;

		//Clear data from match txt file
		outFile.open("C:\\temp\\Text\\BlackJack\\" + strategy + "\\MatchData.txt");
		outFile.close();

		//Clear data from deck txt file
		outFile.open("C:\\temp\\Text\\BlackJack\\" + strategy + "\\DeckData.txt");
		outFile.close();
	}
}


//Primary Methods
void Player::placeBet(string countSystem)
{
	if (countSystem == "High_Low")
	{
		float trueCount = playDeck->getTrueCount();

		if (floor(trueCount - 1) <= 0)
		{
			hand.bet = bettingUnit;
		}
		else
		{
			hand.bet = int(round(trueCount - 1) * bettingUnit);
		}
		if (movesDisplayed)
		{
			cout << "Running Count: " << playDeck->runningCount << ", True Count: " << trueCount << ", Bet: " << hand.bet << endl;

		}
	}
	else
	{
		hand.bet = bettingUnit;
	}
}

void Player::surrender()
{
	surrendered = true;
	if (movesDisplayed)
	{
		cout << "Surrendered\n";
	}
}

void Player::hit(int iterations, bool isSplitHand)
{
	Hand* playingHand = isSplitHand ? &splitHand : &hand;
	for (int i = iterations; i > 0; i--)
	{
		if (playDeck->playSize() <= ((1 - house.pen) * playDeck->packSize * 52)) //If Deck is empty reset
		{
			playDeck->reset();

			//Save deck data and reset gain if not dealer
			if (name != "Dealer")
			{
				saveData("DECK");
				gain = 0;
			}
		}

		//Draw Card from deck and place into hand
		playingHand->heldCards.push_back(playDeck->draw());
	}

	//If hand is bust and contains a high valued ace lower the ace value unless it is a pair of aces
	if (playingHand->isBust() && playingHand->containsHigh_Ace() && !playingHand->isPair())
	{
		playingHand->lowerHigh_Ace();
	}
}

void Player::split()
{
	//Remove the second card from the original hand and make a duplicate
	hand.heldCards.pop_back();
	splitHand = hand;

	if (movesDisplayed)
	{
		cout << "Split\n";
	}

	//Hit both the main hand and split hand so both hands have 2 cards
	hit();
	hit(1, true);
}

void Player::doubleDown(bool isSplitHand)
{
	Hand* playingHand = isSplitHand ? &splitHand : &hand;

	if (movesDisplayed)
	{
		cout << "Doubled Down\n";
	}

	//Double bet then hit once
	playingHand->bet += playingHand->bet;
	hit(1, isSplitHand);
}

void Player::play(Card dealerHand)
{
	if (strategy == "Standard" || strategy == "Dealer")
	{
		standardPlay();
	}
	else if (strategy == "Omnipotent")
	{
		omnipotentPlay();
	}
	else if (strategy == "Basic" || strategy == "Count")
	{
		basicStrategicPlay(dealerHand);
	}
}

void Player::resolve(string handResult, string splitResult)
{
	float matchChange = 0;
	if (surrendered)
	{
		matchChange -= float(hand.bet) / 2;
		surrendered = false;
	}
	else if (handResult == "won")
	{
		matchChange += float(hand.bet) + ((house.BJPayout - 1)*float(hand.bet)*float(hand.isBlackJack()));
	}
	else if (handResult == "lost")
	{
		matchChange -= float(hand.bet);
	}
	else if (handResult == "tie")
	{

	}
	if (splitResult == "won")
	{
		matchChange += float(splitHand.bet) + ((house.BJPayout - 1) * float(splitHand.bet) * float(splitHand.isBlackJack()));
	}
	else if (splitResult == "lost")
	{
		matchChange -= float(splitHand.bet);
	}
	else if (splitResult == "tie")
	{

	}
	gain += matchChange;

	//Display results if specified
	if (movesDisplayed)
	{
		cout << "Hand " << handResult;
		if (!splitHand.heldCards.empty())
		{
			cout << ", Split " << splitResult;
		}
		cout << endl;
		cout << "Match Gain: " << matchChange << ", Total Gain: " << gain << "\n\n";
	}

	//Save deck data if specified
	if (name != "Dealer")
	{
		saveData("MATCH", matchChange, (handResult == "won"), (splitResult == "won"));
	}

	clearHands();
	return;
}

void Player::clearHands()
{
	hand.discardHand(playDeck);
	splitHand.discardHand(playDeck);
	return;
}

void Player::saveData(string type, float matchGain, bool handWon, bool splitWon)
{
	ofstream outFile;
	if (type == "Match" || type == "MATCH")		//Triggered by resolve method
	{
		float deckProgress = 100 - (100 * (playDeck->playSize() / (float(playDeck->packSize) * 52)));
		outFile.open("C:\\temp\\Text\\BlackJack\\" + strategy + "\\MatchData.txt", ios::app);

		outFile << handWon << ' ' << splitWon << ' ' << hand.isBlackJack() << ' '
		<< splitHand.isBlackJack() << ' ' << !splitHand.heldCards.empty() << ' ' 
		<< matchGain << ' ' << deckProgress << ' ';

		outFile.close();
	}
	else if (type == "Deck" || type == "DECK")	//Triggered by hit method when deck is reset
	{
		outFile.open("C:\\temp\\Text\\BlackJack\\" + strategy + "\\DeckData.txt", ios::app);

		outFile << gain << ' ';

		outFile.close();
	}
}



//Troubleshooting methods
void Player::forcedHit(Card forcedCard, int iterations)
{
	for (int i = iterations; i > 0; i--)
	{
		hand.heldCards.push_back(forcedCard);
	}
}

void Player::printGain()
{
	cout << "Gain: " << gain << endl;
}

void Player::printHands()
{
	cout << name << ": ";
	for (Card c : hand.heldCards)
	{
		cout << c.name << " of " << c.suit << ", ";
	}
	cout << "\b\b  " << hand.getVal() << endl;
	if (!splitHand.heldCards.empty())
	{
		cout << name << " Split: ";
		for (Card c : splitHand.heldCards)
		{
			cout << c.name << " of " << c.suit << ", ";
		}
		cout << "\b\b  " << splitHand.getVal() << endl;;
	}
}