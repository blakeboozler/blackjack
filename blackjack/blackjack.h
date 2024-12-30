/** **************************************************************************
 * @file
 *
 * @brief A header file for the Blackjack project. Contains generic and helper
 * function prototypes for the program, along with structure declarations.
 ****************************************************************************/
#pragma once
#include <queue>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <random>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

/** ***************************************************************************
*                     Non-stack Declarations and Prototypes
******************************************************************************/

/**
* @brief Structure that contains face value and suit for 1 card.
*/
struct card
{
    int faceValue; /**< Integer amount for card's face value */
    int suit; /**< Integer amount for card's suit value */
};

/**
* @brief Structure that represents the player, including total tokens and 
* current bet.
*/
struct Player
{
    int totalTokens; /**< The player's total tokens available */
    int bet; /**< The player's current bet amount */

    /**< Player constructor with default tokens of 500 */
    Player(int tokens = 500) : totalTokens(tokens), bet(0) {} 
};


void betMenu(int tokenCount, int& bet);

void roundMenu(queue<card>& deck, queue<card>& pHand, queue<card>& dHand,
    int& whoWon, Player& player);

void displayHands(queue<card>& dHand, queue<card>& pHand, bool initialPhase);

void displayOptions();

bool getValidChoice(int& choice);

void processChoice(int choice, queue<card>& deck, queue<card>& pHand,
    queue<card>& dHand, int& whoWon, Player& player, bool& initialPhase,
    bool& canDoubleDown);

int randNumber();

void generateDeck(queue<card>& deck);

void displayDealerInitial(const queue<card>& dealer);

int sumHand(queue<card>& hand);

int cardCount(queue<card>& hand);

bool checkEarlyWin(queue<card>& pHand, queue<card>& dHand, int& whoWon);

void playerHit(queue<card>& deck, queue<card>& pHand, int& whoWon);

void dealerHit(queue<card>& deck, queue<card>& dHand, int& whoWon);

void doubleDown(queue<card>& deck, queue<card>& pHand, queue<card>& dHand,
    int& whoWon, Player& player);

bool canPurchaseInsurance(queue<card>& dHand, Player& player);

void insuranceOffer(queue<card>& pHand, queue<card>& dHand, int& whoWon,
    Player& player);

int stand(queue<card>& deck, queue<card>& pHand, queue<card>& dHand,
    int& whoWon, int& bet);

void playRound(queue<card>& deck, Player& player);

template<class TY>
ostream& operator<<(ostream& out, queue<TY> q);
