#include <iostream>						// std::cout, std::endl
#include <array>        			// std::array
#include <algorithm>    			// std::shuffle
#include <random>       			// std::default_random_engine
#include <chrono>       			// std::chrono::system_clock
#include <cmath>							// std::max

int main() {
	//deck parameters
	const short cards_per_suit = 10;				//number of cards per suit
	const short suits_per_deck = 4;					//number of suits per deck
	const short num_decks = 1;							//number of decks
	const short num_cards = cards_per_suit*suits_per_deck*num_decks;
	std::array<int,num_cards> deck;

	//creating the deck (card values only)
	std::array<int,cards_per_suit> suit = {2,4,6,8,10,12,14,1,1,1};
	for(int i = 0; i < num_cards; i++) deck.at(i) = suit.at(i % cards_per_suit);
	const short max_points = 15;

	//game variables
	int dealer_hand;
	short dealer_ncards;
	int stands_dealer;
	const int stands_default = 10;
	int player_hand;
	short icards;
	int player_ncards;
	int hand_winnings;
	int sum_winnings = 0;
	int bet_amount = 1;
	char input = 'y';
	int card;
	int firstcard;

	//random seed
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();

	while(input != 'n') {
		//shuffle the deck
		std::shuffle(deck.begin(), deck.end(), std::default_random_engine(seed++));

		//first cards
		icards = 0;
		dealer_hand = deck.at(icards++);
		dealer_ncards = 1;
		firstcard = deck.at(icards++);
		player_hand = firstcard;
		player_ncards = 1;

		//player turns
		std::cout << "Your first card is a " << player_hand / (double) 2 << std::endl;
		while(firstcard == 8) {
			std::cout << "Do you want to burn it (y/n)? ";
			std::cin >> input;
			if(input == 'y') {
				firstcard = deck.at(icards++);
				player_hand = firstcard;
				std::cout << "Your new first card is a " << player_hand / (double) 2 << std::endl;
			}
			if(input == 'n') break;
		}
		std::cout << "How much do you want to bet? ";
		std::cin >> bet_amount;
		if(bet_amount < 1) {
			std::cout << "Invalid amount, bet set to 1" << std::endl;
			bet_amount = 1;
		}
		std::cout << "Hit (h) or Stand (s)? ";
		std::cin >> input;

		while(input == 'h') {
			input = 's';
			card = deck.at(icards++);
			player_hand += card;
			player_ncards++;
			std::cout << "You have been dealt a " << card / (double) 2 << std::endl;
			if(player_hand > max_points) {
				std::cout << "Your hand is now worth " << player_hand / (double) 2 << std::endl;
				std::cout << "You went bust!" << std::endl;
				player_hand = 0;
			}
			else if(player_hand == max_points) {
				if(player_ncards == 2) {
					player_hand = 99;
					std::cout << "You got a royal!" << std::endl;
				}
				else {
					std::cout << "You got 7½!" << std::endl;
				}
			}
			else {
				std::cout << "Your hand is now worth " << player_hand / (double) 2 << std::endl;
				std::cout << "Hit (h) or Stand (s)? ";
				std::cin >> input;
			}
		}

		//dealer turn
		if(player_hand != 0) {
			std::cout << "The dealer reveals the face-down card, it is a " << dealer_hand / (double) 2 << std::endl;
			if (player_hand >= max_points) { stands_dealer = 15; } else {
				stands_dealer = std::max(stands_default,player_hand-firstcard+1);
			}
			while(dealer_hand < stands_dealer) {
				card = deck.at(icards++);
				std::cout << "The dealer hits and receives a " << card / (double) 2 << std::endl;
				dealer_hand += card;
				dealer_ncards++;
			}

			//determine winnings
			//dealer goes bust, non-bust players are paid 1:1, royals are paid 2:1
			if(dealer_hand > max_points) {
					std::cout << "The dealer is bust!" << std::endl;
					if(player_hand == 99) {	hand_winnings = 2*bet_amount; } else { hand_winnings = bet_amount; }
					std::cout << "You are paid " << hand_winnings << std::endl;
			}
			//dealer has a royal, non-royal players pay 2:1, royals pay 1:1
			else if(dealer_hand == max_points && dealer_ncards == 2) {
				std::cout << "The dealer has a royal!" << std::endl;
				if(player_hand == 99) {	hand_winnings = -bet_amount; } else { hand_winnings = -2*bet_amount; }
			}
			//player wins and is paid 1:1, royals are paid 2:1
			else {
				std::cout << "The dealer stands on " << dealer_hand  / (double) 2 << std::endl;
				if(player_hand > dealer_hand) {
					if(player_hand == 99) {	hand_winnings = 2*bet_amount; } else { hand_winnings = bet_amount; }
					std::cout << "You are paid " << hand_winnings << std::endl;
				}
				else {
					std::cout << "You have to pay " << bet_amount << std::endl;
					hand_winnings = -bet_amount;
				}
			}
		}
		else {
			std::cout << "You have to pay " << bet_amount << std::endl;
			hand_winnings = -bet_amount;
		}

		sum_winnings += hand_winnings;
		std::cout << "Your total winnings are " << sum_winnings << std::endl;
		std::cout << "Another round (y/n)? ";
		std::cin >> input;
	}

	return 0;
}
