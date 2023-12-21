#include "Hand.h"

//Primary Method Definitions//
int Hand::getVal()
{
    int total = 0;
    for (Card c : heldCards)
    {
        total += c.value;
    }
    return total;
}

bool Hand::isBust()
{
    return getVal() > 21;
}

bool Hand::isBlackJack()
{
    return (heldCards.size() == 2 && getVal() == 21);
}

bool Hand::isPair()
{
    return ((heldCards.size() == 2) && (heldCards[0].name == heldCards[1].name));
}

bool Hand::isSoft17()
{
    return (heldCards.size() == 2 && containsHigh_Ace() && getVal() == 6);
}

bool Hand::containsLow_Ace()
{
    for (Card c : heldCards)
    {
        if (c.name == "Low_Ace")
        {
            return true;
        }
    }
    return false;
}

bool Hand::containsHigh_Ace()
{
    for (Card c : heldCards)
    {
        if (c.name == "High_Ace")
        {
            return true;
        }
    }
    return false;
}

void Hand::raiseLow_Ace()
{
    for (int i = 0; i < int(heldCards.size()); i++)
    {
        if (heldCards[i].name == "Low_Ace")
        {
            heldCards[i].name = "High_Ace";
            heldCards[i].value = 11;
            return;
        }
    }
}

void Hand::lowerHigh_Ace()
{
    for (int i = 0; i < int(heldCards.size()); i++)
    {
        if (heldCards[i].name == "High_Ace")
        {
            heldCards[i].name = "Low_Ace";
            heldCards[i].value = 1;
            return;
        }
    }
}

void Hand::discardHand(Pack* deck)
{
    if (!heldCards.empty())
    {
        for (Card c : heldCards)
        {
            deck->discard(c);
        }
        heldCards.clear();
        bet = 0;
    }
    return;
}