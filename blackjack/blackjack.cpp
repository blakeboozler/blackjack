/** **********************************************************************
* @file
*
* @brief This file contains the definitions for regular functions
*        of the Blackjack game, with the game logic and player interaction.
************************************************************************/
/** ***************************************************************************
* @mainpage Blackjack - Card Game
*
* @section Personal Project
*
* @author Blake Mommaerts
*
* @date 01/01/2025
*
* @details This program is designed to simulate the card game Blackjack. In 
* this variation, the game is played between the player and a dealer, 
* where each hand consists of two initial cards. The goal is to get as 
* close to 21 as possible without exceeding it. Face cards (Jack, Queen, King) 
* are valued at 10 points, and Aces can be worth either 1 or 11 points 
* depending on the player's hand. The player can choose to "Hit" 
* (draw another card) or "Stand" (end their turn). If the player's hand exceeds
* 21 points, they "Bust" and lose the round. The dealer also plays under the 
* rule of hitting until they reach a hand total of at least 17. If the dealer 
* busts, the player wins. If neither player busts, the player with the higher 
* total wins the round. If both have the same total, it is a tie or "Push."
*
* The game continues until the player chooses to quit, and the total amount of
* winnings or losses is displayed at the end.
*
* @section compiling_section Compiling and Usage
* @par Compiling Instructions:
*      None
*
* @section todo_bugs_modification_section Modifications & Development Timeline
*
* @todo Implement functionality for splitting hands (up to 4 times).
* @todo Add support for multiple rounds and player choices.
* @todo Make optimizations to code and seek out reduction of total variables.
*
* @bug None
*
* @par Timeline
* Visit the project repository for timeline updates and commit logs.
* <a href =
* "https://github.com/blakeboozler/blackjack"
* target="_blank">Click Here.</a>
*****************************************************************************/

#include "blackjack.h"

/** ***************************************************************************
*                              Main + Definitions
******************************************************************************/

/** **********************************************************************
 * @brief Simulates a game of Blackjack with a player and deck of cards.
 *
 * @details The main function of the Blackjack game. It prompts the player
 *          with options to either play a round or quit the game. The game
 *          continues until the player chooses to quit or runs out of tokens.
 *          It handles betting, shuffling a deck of cards, and playing rounds.
 *          The player's total tokens are tracked throughout the game.
 *
 * @returns 0 when the function exits upon successful completion of the game.
 *
 * @par Example
 * @code{.cpp}
 * main();
 * @endcode
 ************************************************************************/
int main() 
{
    int choice;
    queue<card> deck;
    Player player(500); // Initialize player with 500 tokens

    do 
    {
        cout << "Total Tokens: " << player.totalTokens << endl;
        cout << "   1) Play Round " << endl;
        cout << "   2) Quit " << endl;
        cout << "Enter Choice: ";

        while (!(cin >> choice) || choice > 2 || choice < 0) 
        {
            cout << "Incorrect option. Please specify 1 or 2." << endl;
            cin.clear();
            cin.ignore(256, '\n');
        }

        cout << endl;

        switch (choice) 
        {
            case 1:
                betMenu(player.totalTokens, player.bet);
                generateDeck(deck);
                playRound(deck, player);
                player.totalTokens += player.bet;
                break;
            case 2:
                cout << "Total tokens: " << player.totalTokens << endl;
                break;
        }
    } while (choice != 2 && !(player.totalTokens < 10));

    if (player.totalTokens < 10)
        cout << "Out of tokens - game over!" << endl;

    return 0;
}

/** **********************************************************************
 * @brief Displays the betting menu and prompts the player for a valid bet.
 *
 * @details This function displays the player's total tokens and prompts
 *          them to enter a bet. It validates the bet to ensure it is a
 *          multiple of 10, within the player's total token count, and at
 *          least 10 tokens. If the input is invalid, the function requests
 *          a new input until a valid bet is entered.
 *
 * @param[in] tokenCount The current number of tokens the player has.
 * @param[out] bet The amount the player chooses to bet. This value is
 *                 modified within the function.
 *
 * @par Example
 * @code{.cpp}
 * int playerTokens = 500;
 * int betAmount;
 * betMenu(playerTokens, betAmount);
 * @endcode
 ************************************************************************/
void betMenu(int tokenCount, int& bet)
{
    bet = 0;
    do
    {
        cout << "Total tokens: " << tokenCount << endl;
        cout << "Your bet: ";

        while (!(cin >> bet) || (bet % 10) != 0 
            || bet < 10 || bet > tokenCount)
        {
            cout << "Insufficient bet. Must be a min and/or increment of 10, "
                << "and within your total." << endl;
            cin.clear();
            cin.ignore(256, '\n');
            // ^ clearing the false input and preparing to loop
        }
        cout << endl;
    } while (bet == 0);
}

/** **********************************************************************
 * @brief Handles the round menu for a Blackjack game round, allowing the 
 *        player to make decisions during their turn.
 *
 * @details This function controls the flow of a single round of Blackjack,
 *          displaying the player's and dealer's hands, presenting the player
 *          with options to hit, stand, or double down, and processing the 
 *          player's choices. It also checks for early wins and calls the 
 *          appropriate functions based on the player's actions.
 *
 * @param[in,out] deck The deck of cards being used for the game. This is 
 *                     modified during the round as cards are drawn.
 * @param[in,out] pHand The player's hand. This is modified as the player draws
 *                      cards or stands.
 * @param[in,out] dHand The dealer's hand. This is modified as cards are drawn.
 * @param[out] whoWon A reference to an integer representing the outcome of the
 *                    round. Set to 1 if the player wins, 3 if the dealer wins, 
 *                    2 if a push, or 0 if the round continues.
 * @param[in,out] player The player object, which tracks the player's tokens 
 *                       and bet.
 *
 * @par Example
 * @code{.cpp}
 * queue<card> deck, playerHand, dealerHand;
 * int roundResult = 0;
 * Player player(500);
 * roundMenu(deck, playerHand, dealerHand, roundResult, player);
 * @endcode
 ************************************************************************/
void roundMenu(queue<card>& deck, queue<card>& pHand, queue<card>& dHand, 
    int& whoWon, Player& player) 
{
    int choice;
    bool initialPhase = true;
    bool canDoubleDown = true;

    do 
    {
        displayHands(dHand, pHand, initialPhase);

        if (checkEarlyWin(pHand, dHand, whoWon)) 
        {
            whoWon = stand(deck, pHand, dHand, whoWon, player.bet);
            return;
        }

        displayOptions();

        if (!getValidChoice(choice)) continue;

        processChoice(choice, deck, pHand, dHand, whoWon, player, initialPhase,
            canDoubleDown);

    } while (choice != 3 && whoWon == 0);
}

/** **********************************************************************
 * @brief Displays the current hands of the dealer and the player.
 *
 * @details This function outputs the dealer's and player's hands to the 
 * console.During the initial phase of the game, the dealer’s hand is displayed
 * partially to hide one of the cards. After the initial phase, the full 
 * dealer’s hand is revealed. The player's hand is always shown along with 
 * the total value of the hand.
 *
 * @param[in] dHand The dealer's hand, represented as a queue of cards.
 * @param[in] pHand The player's hand, represented as a queue of cards.
 * @param[in] initialPhase A boolean flag indicating whether the game is in 
 *                         the initial phase (before the dealer reveals 
 *                         both cards).
 *
 * @par Example
 * @code{.cpp}
 * queue<card> dealerHand, playerHand;
 * bool gamePhase = true;
 * displayHands(dealerHand, playerHand, gamePhase);
 * @endcode
 ************************************************************************/
void displayHands(queue<card>& dHand, queue<card>& pHand, bool initialPhase) 
{
    cout << "Dealer: ";
    if (initialPhase)
        displayDealerInitial(dHand);
    else
        cout << dHand << endl;

    cout << "Player: " << pHand << " (" << sumHand(pHand) << ")" << endl;
}

/** **********************************************************************
 * @brief Displays the available options for the player during their turn.
 *
 * @details This function outputs the available choices for the player during
 *          their turn in a Blackjack game. The player is prompted to either
 *          "Hit", "Double Down", or "Stand" in order to proceed with their 
 *          turn.
 *
 * @par Example
 * @code{.cpp}
 * displayOptions();
 * @endcode
 ************************************************************************/
void displayOptions() 
{
    cout << "   1) Hit " << endl;
    cout << "   2) Double Down " << endl;
    cout << "   3) Stand " << endl;
    cout << "Enter Choice: ";
}

/** **********************************************************************
 * @brief Prompts the player for a valid choice and ensures the input is within
 *        the acceptable range.
 *
 * @details This function takes input from the player and ensures that the
 *          choice is a valid number between 1 and 3. If the input is invalid,
 *          it displays an error message, clears the input buffer, and returns
 *          false to indicate that a valid input was not provided. Otherwise,
 *          it returns true to indicate the input was valid.
 *
 * @param[out] choice The player's choice. This value is modified to reflect
 *                    the user's input if the input is valid.
 *
 * @returns `true` if the player enters a valid choice (1-3), `false` 
 *          otherwise.
 *
 * @par Example
 * @code{.cpp}
 * int playerChoice;
 * if (getValidChoice(playerChoice)) {
 *     // Proceed with the game logic based on playerChoice
 * }
 * @endcode
 ************************************************************************/
bool getValidChoice(int& choice) 
{
    if (!(cin >> choice) || choice < 1 || choice > 3) 
    {
        cout << "Incorrect option. Please specify a number 1-3." << endl;
        cin.clear();
        cin.ignore(256, '\n');
        return false;
    }
    return true;
}

/** **********************************************************************
 * @brief Processes the player's choice during their turn in the Blackjack 
 *        game.
 *
 * @details This function handles the player's actions based on their menu 
 *          choice during their turn in the Blackjack game. It updates the
 *          game state accordingly, such as allowing the player to hit, double
 *          down, or stand. It also checks conditions like whether the player
 *          can double down and whether the insurance option is available. 
 *          The function modifies relevant game variables, such as the player's
 *          hand, the dealer's hand,and the round outcome.
 *
 * @param[in] choice The player's choice, indicating their action (1: Hit, 2: 
 *                   Double Down, 3: Stand).
 * @param[in,out] deck The deck of cards being used for the game. It is 
 *                     modified as cards are drawn.
 * @param[in,out] pHand The player's hand. It is modified when the player hits
 *                      or doubles down.
 * @param[in,out] dHand The dealer's hand. It is modified if the insurance
 *                      option is chosen.
 * @param[in,out] whoWon A reference to an integer representing the outcome of
 *                       the round. Set to 1 if the player wins, -1 if the 
 *                       dealer wins, or 0 if the round continues.
 * @param[in,out] player The player object, which tracks the player's tokens 
 *                       and bet.
 * @param[in,out] initialPhase A reference to a boolean flag indicating whether
 *                             the game is in the initial phase (before the 
 *                             dealer reveals both cards).
 * @param[in,out] canDoubleDown A boolean flag that indicates whether the 
 *                              player is allowed to double down.
 *
 * @par Example
 * @code{.cpp}
 * queue<card> deck, playerHand, dealerHand;
 * int roundResult = 0;
 * Player player(500);
 * bool gamePhase = true, doubleDownAllowed = true;
 * processChoice(1, deck, playerHand, dealerHand, roundResult, player,
 * gamePhase, doubleDownAllowed);
 * @endcode
 ************************************************************************/
void processChoice(int choice, queue<card>& deck, queue<card>& pHand, 
    queue<card>& dHand, int& whoWon, Player& player, bool& initialPhase, 
    bool& canDoubleDown) 
{
    switch (choice) 
    {
        case 1:
            playerHit(deck, pHand, whoWon);
            canDoubleDown = false;
            cout << endl;
            break;
        case 2:
            if (canDoubleDown) 
            {
                doubleDown(deck, pHand, dHand, whoWon, player);
                initialPhase = false;
                cout << endl;
            }
            else 
            {
                cout << endl;
                cout << "You can't double down anymore!" << endl;
                cout << endl;
            }
            break;
        case 3:
            if (canPurchaseInsurance(dHand, player))
                insuranceOffer(pHand, dHand, whoWon, player);
            whoWon = stand(deck, pHand, dHand, whoWon, player.bet);
            initialPhase = false;
            cout << endl;
            break;
    }
}

/** **********************************************************************
 * @brief Generates a random integer in the range [1, 100].
 *
 * @details This function generates a random number in the inclusive range 
 *          [1, 100] using a non-deterministic random seed obtained from a
 *          random device and a Mersenne Twister engine. The random number
 *          is returned as an integer.
 *
 * @returns A random integer between 1 and 100 (inclusive).
 *
 * @par Example
 * @code{.cpp}
 * int num = randNumber(); // Generates a random number between 1 and 100
 * cout << "Random number: " << num << endl;
 * @endcode
 ************************************************************************/
int randNumber()
{
    // Seed with a random device for non-deterministic randomness
    random_device rd;

    // Use a Mersenne Twister random number generator
    mt19937 gen(rd());

    // Define a uniform integer distribution for the range [1, 100]
    uniform_int_distribution<> dis(1, 100);

    // Generate a random number
    int randomNumber = dis(gen);

    return randomNumber;
}

/** **********************************************************************
 * @brief Generates a shuffled deck of 52 unique cards.
 *
 * @details This function creates a deck of 52 unique cards, ensuring each card
 *          is randomly selected without repetition. The deck is generated by
 *          using a random engine and a uniform integer distribution to select
 *          card indices. It then populates a queue with `card` objects 
 *          representing the cards, ensuring that the card values (face and 
 *          suit) are unique across the deck. The function uses a boolean 
 *          vector to track used cards and prevent duplicates.
 *
 * @param[out] deck The queue where the generated deck of cards is stored.
 *                  Each card is represented by a `card` object with a face 
 *                  value and suit.
 *
 * @par Example
 * @code{.cpp}
 * queue<card> deck;
 * generateDeck(deck); // Generates a shuffled deck and stores it in the queue
 * @endcode
 ************************************************************************/
void generateDeck(queue<card>& deck)
{
    card aCard;
    vector<bool> used(52, false); // Automatically initializes all to false

    // Random card generator using random engine
    default_random_engine generator(randNumber()); //71, 184
    uniform_int_distribution<int> distribution(0, 51);

    for (int i = 0; i < 52; i++)
    {
        int cardValue = distribution(generator);
        // Ensure cardValue is unique
        while (used[cardValue])
        {
            cardValue = distribution(generator);
        }

        aCard.faceValue = (cardValue % 13) + 1; // Obtain face value
        aCard.suit = cardValue / 13;           // Obtain suit value
        used[cardValue] = true;                // Mark as used
        deck.push(aCard);                      // Enqueue the unique card
    }
}

/** **********************************************************************
 * @brief Displays the dealer's first card in a shortened format.
 *
 * @details This function prints the face value and suit of the dealer's first
 *          card in a shortened format. The face value is represented by its
 *          corresponding letter (A for Ace, J for Jack, Q for Queen, K for
 *          King) and the suit is represented by a single letter (H for Hearts,
 *          D for Diamonds, C for Clubs, S for Spades). The card is displayed
 *          as "XX" to hide the second card in the dealer's hand.
 *
 * @param[in] dealer A constant reference to a queue of `card` objects, where
 *                   each `card` contains a face value and suit.
 *
 * @par Example
 * @code{.cpp}
 * queue<card> dealer;
 * // Assuming dealer's queue is populated
 * displayDealerInitial(dealer); 
 * // Displays the dealer's first card in a shortened format
 * @endcode
 ************************************************************************/

void displayDealerInitial(const queue<card>& dealer)
{
    queue<card> dealerCopy = dealer;
    card firstCard = dealerCopy.front(); // Get the first card

    if (firstCard.faceValue == 1)
        cout << "A";
    else if (firstCard.faceValue == 11)
        cout << "J";
    else if (firstCard.faceValue == 12)
        cout << "Q";
    else if (firstCard.faceValue == 13)
        cout << "K";
    else
        cout << firstCard.faceValue;

    if (firstCard.suit == 0)
        cout << "H";
    else if (firstCard.suit == 1)
        cout << "D";
    else if (firstCard.suit == 2)
        cout << "C";
    else if (firstCard.suit == 3)
        cout << "S";

    cout << " XX" << endl;
}

/** **********************************************************************
 * @brief Sums the values of a hand of cards.
 *
 * @details This function calculates the total value of a hand in Blackjack.
 *          It takes into account face cards (J, Q, K) which are worth 10,
 *          and aces which are worth 11 unless the sum exceeds 21, in which
 *          case aces are adjusted to 1. The function handles multiple aces
 *          correctly by adjusting the total sum to avoid busting.
 *
 * @param[in] hand A reference to a queue of `card` objects representing the
 *                 player's or dealer's hand.
 *
 * @returns The sum of the hand's values, with ace adjustments if necessary.
 *
 * @par Example
 * @code{.cpp}
 * queue<card> hand;
 * // Assuming hand is populated
 * int handValue = sumHand(hand); // Returns the sum of the hand's values
 * @endcode
 ************************************************************************/
int sumHand(queue<card>& hand)
{
    queue<card> handCopy = hand; // Make a copy to iterate through
    card aCard;
    int handSum = 0;
    int aceCount = 0;

    while (!handCopy.empty())
    {
        aCard = handCopy.front();
        handCopy.pop();

        if (aCard.faceValue > 10)
            aCard.faceValue = 10; // Face cards (J, Q, K) are worth 10
        else if (aCard.faceValue == 1)
        {
            aCard.faceValue = 11;
            aceCount++; // Count the number of aces
        }

        handSum += aCard.faceValue;
    }

    // Adjust for aces
    while (handSum > 21 && aceCount > 0)
    {
        handSum -= 10; // Convert one ace from 11 to 1
        aceCount--;
    }

    return handSum;
}

/** **********************************************************************
 * @brief Counts the number of cards in a hand.
 *
 * @details This function iterates through the given hand, counting the
 *          number of cards in the hand and returning the total count.
 *
 * @param[in] hand A reference to a queue of `card` objects representing the
 *                 player's or dealer's hand.
 *
 * @returns The total number of cards in the hand.
 *
 * @par Example
 * @code{.cpp}
 * queue<card> hand;
 * // Assuming hand is populated
 * int numCards = cardCount(hand); // Returns the total number of cards
 * @endcode
 ************************************************************************/
int cardCount(queue<card>& hand)
{
    int cardTotal = 0;
    queue<card> handCopy = hand;
    while (!handCopy.empty())
    {
        cardTotal++;
        handCopy.pop();
    }
    return cardTotal;
}

/** **********************************************************************
 * @brief Checks for an early win (Blackjack) for the player or dealer.
 *
 * @details This function checks whether either the player or dealer has
 *          a total of 21 points (Blackjack) at the start of the round.
 *          If both have 21 points, it results in a tie (push). If only the
 *          player has 21, they win automatically. If the dealer has 21,
 *          it results in a push.
 *
 * @param[in] player A reference to a queue of `card` objects representing
 *                   the player's hand.
 * @param[in] dealer A reference to a queue of `card` objects representing
 *                   the dealer's hand.
 * @param[out] whoWon A reference to an integer that holds the result of
 *                    the game (0 = player wins, 1 = dealer wins, 2 = tie).
 *
 * @returns `true` if an early win (Blackjack) was detected, `false` otherwise.
 *
 * @par Example
 * @code{.cpp}
 * queue<card> player, dealer;
 * // Assuming hands are populated
 * int winner;
 * bool isEarlyWin = checkEarlyWin(player, dealer, winner);
 * @endcode
 ************************************************************************/
bool checkEarlyWin(queue<card>& player, queue<card>& dealer, int& whoWon)
{
    // Check for initial 21 (Blackjack) for player or dealer
    int pSum = sumHand(player);
    int dSum = sumHand(dealer);

    if (pSum == 21)
    {
        if (pSum == 21 && dSum == 21)
        {
            whoWon = 2; // Push (tie)
        }
        else if (pSum == 21)
        {
            whoWon = 0; // Player Blackjack, automatic stand
        }
        else if (dSum == 21)
        {
            whoWon = 0; // Dealer Blackjack, push at best
        }
        cout << endl;
        return true;
    }
    return false;
}

/** **********************************************************************
 * @brief Deals a card to the player and checks for a win or bust.
 *
 * @details This function pops a card from the deck and adds it to the
 *          player's hand. It then checks if the player's hand exceeds
 *          21, in which case the player busts, or if the hand equals
 *          21, in which case the player wins or pushes.
 *
 * @param[in,out] deck A reference to a queue of `card` objects representing
 *                     the deck of cards. The top card is drawn when the
 *                     player hits.
 * @param[in,out] player A reference to a queue of `card` objects representing
 *                       the player's hand. A card is added to the player's
 *                       hand when they hit.
 * @param[out] whoWon A reference to an integer that holds the result of
 *                    the game (0 = player wins, 1 = dealer wins, 3 = player
 *                    busts).
 *
 * @par Example
 * @code{.cpp}
 * queue<card> deck, player;
 * int winner;
 * playerHit(deck, player, winner);
 * @endcode
 ************************************************************************/
void playerHit(queue<card>& deck, queue<card>& player, int& whoWon)
{
    card deckCard = deck.front();
    deck.pop();
    player.push(deckCard);

    if (sumHand(player) > 21)
        whoWon = 3; // Player bust
    if (sumHand(player) == 21)
        whoWon = 0; // Player wins or pushes
}

/** **********************************************************************
 * @brief Deals a card to the dealer and checks for a win or bust.
 *
 * @details This function pops a card from the deck and adds it to the
 *          dealer's hand. It then checks if the dealer's hand exceeds
 *          21, in which case the dealer busts, or if the hand equals
 *          21, in which case the dealer wins or pushes.
 *
 * @param[in,out] deck A reference to a queue of `card` objects representing
 *                     the deck of cards. The top card is drawn when the
 *                     dealer hits.
 * @param[in,out] dealer A reference to a queue of `card` objects representing
 *                       the dealer's hand. A card is added to the dealer's
 *                       hand when they hit.
 * @param[out] whoWon A reference to an integer that holds the result of
 *                    the game (0 = player wins, 1 = dealer wins, 3 = player
 *                    busts).
 *
 * @par Example
 * @code{.cpp}
 * queue<card> deck, dealer;
 * int winner;
 * dealerHit(deck, dealer, winner);
 * @endcode
 ************************************************************************/
void dealerHit(queue<card>& deck, queue<card>& dealer, int& whoWon)
{
    card deckCard = deck.front();
    deck.pop();
    dealer.push(deckCard);

    if (sumHand(dealer) > 21)
        whoWon = 1; // Dealer bust
    if (sumHand(dealer) == 21)
        whoWon = 0; // Dealer wins or pushes
}

/** **********************************************************************
 * @brief Doubles the player's bet and deals one more card to the player.
 *
 * @details This function allows the player to double down, which means
 *          doubling their bet and receiving one more card. If the player
 *          doesn't have enough tokens to double the bet, an error message
 *          is displayed. After the card is dealt, the player stands, and
 *          the bet is adjusted based on the outcome.
 *
 * @param[in,out] deck A reference to a queue of `card` objects representing
 *                     the deck. A card is drawn when the player doubles down.
 * @param[in,out] pHand A reference to a queue of `card` objects representing
 *                      the player's hand. A card is added to the player's
 *                      hand when they double down.
 * @param[in,out] dHand A reference to a queue of `card` objects representing
 *                      the dealer's hand. No changes are made to the dealer's
 *                      hand in this function.
 * @param[out] whoWon A reference to an integer that holds the result of
 *                    the game (0 = player wins, 1 = dealer wins, 3 = player
 *                    busts).
 * @param[in,out] player A reference to a `Player` object representing the
 *                       player. The bet is doubled if the player chooses to
 *                       double down and has enough tokens.
 *
 * @par Example
 * @code{.cpp}
 * queue<card> deck, player, dealer;
 * int winner;
 * Player p(100);
 * doubleDown(deck, player, dealer, winner, p);
 * @endcode
 ************************************************************************/
void doubleDown(queue<card>& deck, queue<card>& pHand, 
    queue<card>& dHand, int& whoWon, Player& player) 
{
    if (player.totalTokens >= (player.bet * 2)) 
    {
        playerHit(deck, pHand, whoWon);
        whoWon = stand(deck, pHand, dHand, whoWon, player.bet);
        // The bet doubles, will be positive or negative in standing phase
        if (whoWon == 1 || whoWon == 3)
            player.bet = player.bet * 2;
    }
    else
        cout << "Not enough to double down!" << endl;
}

/** **********************************************************************
 * @brief Checks if the player can purchase insurance based on their
 *        current bet and the dealer's upcard.
 *
 * @details This function determines if the player can purchase insurance
 *          based on the dealer's upcard (Ace) and the player's remaining
 *          tokens. The player can buy insurance if they have enough tokens
 *          to wager half of their original bet and the dealer's upcard is an
 *          Ace.
 *
 * @param[in] dHand A reference to a queue of `card` objects representing
 *                  the dealer's hand. The first card is used to check for
 *                  an Ace.
 * @param[in] player A reference to a `Player` object. The function checks
 *                   the player's remaining tokens and bet to determine
 *                   if they can purchase insurance.
 *
 * @return Returns `true` if the player can purchase insurance, otherwise
 *         `false`.
 *
 * @par Example
 * @code{.cpp}
 * queue<card> dealerHand;
 * Player p(100);
 * bool canBuy = canPurchaseInsurance(dealerHand, p);
 * @endcode
 ************************************************************************/
bool canPurchaseInsurance(queue<card>& dHand, Player& player)
{
    int pTokens = player.totalTokens;
    int pBet = player.bet;
    card aCard;
    aCard = dHand.front();
    if (aCard.faceValue == 1 && 
        (player.totalTokens-player.bet) >= (player.bet / 2))
        return true;
    else
        return false;
}

/** **********************************************************************
 * @brief Prompts the player to purchase insurance and adjusts the bet
 *        accordingly based on the dealer's hand and the outcome of the
 *        game.
 *
 * @details This function displays an option to the player to purchase
 *          insurance when the dealer shows an Ace. If the player chooses
 *          to purchase insurance, the bet is adjusted based on the game
 *          result. The player's bet is either refunded, reduced, or
 *          increased depending on the dealer's hand and the game's outcome.
 *
 * @param[in] pHand A reference to a queue of `card` objects representing
 *                  the player's hand.
 * @param[in] dHand A reference to a queue of `card` objects representing
 *                  the dealer's hand.
 * @param[inout] whoWon A reference to an integer representing the outcome
 *                      of the round. It is updated based on the game result.
 * @param[inout] player A reference to a `Player` object representing the
 *                      player. The player's bet is modified if they purchase
 *                      insurance.
 *
 * @par Example
 * @code{.cpp}
 * queue<card> playerHand, dealerHand;
 * Player p(100);
 * insuranceOffer(playerHand, dealerHand, whoWon, p);
 * @endcode
 ************************************************************************/
void insuranceOffer(queue<card>& pHand, queue<card>& dHand, int& whoWon, 
    Player& player)
{
    int choice = 0;
    do 
    {
        cout << endl;
        cout << "Would you like to purchase insurance? " << endl;
        cout << "   1) Yes " << endl;
        cout << "   2) No " << endl;
        cout << "Enter Choice: ";

        while (!(cin >> choice) || choice > 2 || choice < 0) 
        {
            cout << "Incorrect option. Please specify 1 or 2." << endl;
            cin.clear();
            cin.ignore(256, '\n');
        }

        switch (choice) 
        {
            case 1:
                if (whoWon == 0 && sumHand(dHand) == 21)
                    player.bet = 0;
                else if (whoWon == 0 && sumHand(dHand) == 21 
                    && sumHand(pHand) == 21)
                    player.bet = player.bet;
                else if (whoWon == 3)
                    player.bet = (player.bet * 3) / 2;
                else
                    player.bet = player.bet - player.bet / 2;
                break;
            case 2:
                break;
        }
    } while (choice == 0 && player.totalTokens != 0);
}

/** **********************************************************************
 * @brief Determines the outcome of the player's turn by having the dealer
 *        hit until their hand value reaches 17 or higher, and then compares
 *        the player's hand with the dealer's to decide the winner.
 *
 * @details This function simulates the dealer's turn, where the dealer
 *          continues to hit until their hand value is at least 17. It then
 *          compares the player's hand to the dealer's hand to determine the
 *          winner. The result can be a win, loss, tie, or push, depending on
 *          the hand values.
 *
 * @param[in] deck A reference to a queue of `card` objects representing the
 *                 deck of cards.
 * @param[in] pHand A reference to a queue of `card` objects representing the
 *                  player's hand.
 * @param[in] dHand A reference to a queue of `card` objects representing the
 *                  dealer's hand.
 * @param[inout] whoWon A reference to an integer representing the outcome of
 *                      the round. It is updated based on the game result.
 * @param[inout] bet A reference to the player's bet. It is used to calculate
 *                   the payout based on the outcome.
 *
 * @return The outcome of the round:
 *         - 1 for player win
 *         - 2 for push (tie)
 *         - 3 for player loss
 *         - -1 for undefined error
 *
 * @par Example
 * @code{.cpp}
 * queue<card> playerHand, dealerHand;
 * int whoWon = 0;
 * int bet = 50;
 * stand(deck, playerHand, dealerHand, whoWon, bet);
 * @endcode
 ************************************************************************/
int stand(queue<card>& deck, queue<card>& pHand, queue<card>& dHand, 
    int& whoWon, int& bet)
{
    int dBust = 0;
    int pSum = 0;
    int dSum = 0;

    if (whoWon != 0)
        return whoWon;

    pSum = sumHand(pHand);
    dSum = sumHand(dHand);

    while (dSum < 17)
    {
        dealerHit(deck, dHand, whoWon);
        dSum = sumHand(dHand);
    }
    
    if (whoWon != 0)
        return whoWon;

    if (pSum > dSum)
        return 1;
    else if (pSum == dSum)
        if (pSum == 21 && dSum == 21 && (cardCount(pHand) > cardCount(dHand)))
            return 3;
        else
            return 2;
    else if (pSum < dSum)
        return 3;
    else
        return -1;
}

/** **********************************************************************
 * @brief Simulates a round of the game, where the player and dealer are
 *        dealt cards and the winner is determined based on the player's
 *        choices and the final hand values.
 *
 * @details This function manages the flow of a single round of the game. It
 *          deals two cards to both the player and the dealer, displays the
 *          round menu, and determines the winner based on the hand values.
 *          The player's bet is adjusted accordingly based on the outcome of
 *          the round.
 *
 * @param[inout] deck A reference to a queue of `card` objects representing
 *                    the deck of cards.
 * @param[inout] player A reference to the `Player` object representing the
 *                      current player.
 *
 * @return None
 *
 * @par Example
 * @code{.cpp}
 * queue<card> deck;
 * Player player;
 * playRound(deck, player);
 * @endcode
 ************************************************************************/
void playRound(queue<card>& deck, Player& player) 
{
    int whoWon = 0;
    card deckCard;
    queue<card> pHand, dHand;

    if (!deck.empty()) 
    {
        for (int i = 0; i < 2; i++) 
        {
            deckCard = deck.front();
            deck.pop();
            pHand.push(deckCard);

            deckCard = deck.front();
            deck.pop();
            dHand.push(deckCard);
        }

        roundMenu(deck, pHand, dHand, whoWon, player);

        if (whoWon == 1) 
        {
            if (sumHand(pHand) == 21 && cardCount(pHand) == 2)
                player.bet = (player.bet * 3) / 2;
            cout << "Player won" << endl;
        }
        else if (whoWon == 2) 
        {
            player.bet = 0;
            cout << "Push" << endl;
        }
        else if (whoWon == 3) 
        {
            player.bet *= -1;
            cout << "Dealer won" << endl;
        }
        cout << "Dealer: " << dHand << "(" << sumHand(dHand) << ")" << endl;
        cout << "Player: " << pHand << "(" << sumHand(pHand) << ")" << endl;
        cout << endl;
    }
}

/** **********************************************************************
 * @brief Overloads the << operator to print the contents of a queue of
 *        `card` objects.
 *
 * @details This function allows the `queue<card>` type to be printed to an
 *          output stream. It iterates through the queue and prints each card's
 *          face value and suit. Face values of 1, 11, 12, and 13 are replaced
 *          with "A", "J", "Q", and "K" respectively. Suits are represented
 *          by "H", "D", "C", and "S" for Hearts, Diamonds, Clubs, and
 *          Spades, respectively.
 *
 * @param[in,out] out The output stream to which the queue of cards will be
 *                    printed.
 * @param[in] q The queue of `card` objects to be printed.
 *
 * @return A reference to the output stream.
 *
 * @par Example
 * @code{.cpp}
 * queue<card> cards;
 * cout << cards;
 * @endcode
 ************************************************************************/
template<class TY>
ostream& operator<<(ostream& out, queue<TY> q)
{
    TY data;

    if (q.empty())
        return out;

    while (!(q.empty()))
    {
        data = q.front();

        if ((data.faceValue) == 1) // Changing 1 to A for Ace
            out << "A";
        else if ((data.faceValue) == 11) // Changing 11 to J for Jack
            out << "J";
        else if ((data.faceValue) == 12) // Changing 12 to Q for Queen
            out << "Q";
        else if ((data.faceValue) == 13) // Changing 13 to K for King
            out << "K";
        else
            out << (data.faceValue); // Output the card # otherwise

        if (data.suit == 0) // Suit 0 is Hearts
            out << "H";
        if (data.suit == 1) // Suit 1 is Diamonds
            out << "D";
        if (data.suit == 2) // Suit 2 is Clubs
            out << "C";
        if (data.suit == 3) // Suit 3 is Spades
            out << "S";

        out << " ";
        q.pop();
    }

    return out;
}