#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // For getpid function

typedef enum 
{
    SPADES,            //0
    HEARTS,            //1
    DIAMONDS,          //2
    CLUBS              //3
} SUIT;

typedef struct 
{
    int value; // 1 for ace, 2-10 for number cards, 11 for Jack, 12 for Queen, 13 for King
    SUIT suit;
} CARD;

typedef struct 
{
    CARD cards[4]; // players can hold a maximum of 4 cards
} HAND;

typedef struct 
{
    CARD cards[20]; // board can hold 20 cards max
} BOARD;

typedef struct 
{
    CARD cards[52]; // deck of 52 cards
    int num_cards; // number of cards left in the deck
} DECK;

void initial_deck(DECK *deck) 
{
    int i = 0; // index of card in deck
    for (int value = 1; value <= 13; value++) 
    {
        for (int suit = SPADES; suit <= CLUBS; suit++) 
        {
            (*deck).cards[i].value = value;
            (*deck).cards[i].suit = suit;
            i++;
        }
    }
    (*deck).num_cards = 52;
}

void reset_game_state(DECK *deck, HAND players[], BOARD *board) 
{
    // clear the board
    for (int i = 0; i < 4; i++) 
    {
        (*board).cards[i].value = 0;
        (*board).cards[i].suit = -1;
    }

    // clear players hands
    for (int i = 0; i < 4; i++) 
    {
        for (int j = 0; j < 4; j++) 
        {
            players[i].cards[j].value = 0;
            players[i].cards[j].suit = -1;
        }
    }

    // refill the deck
    initial_deck(deck);
    (*deck).num_cards = 52;
}

void shuffle_deck(DECK *deck) 
{
    int j;
    srand(time(NULL) | getpid());
    //i used time and process ID to make random choise as dynamic as possible, 
    //because if i used int value the random is more likely to be algorithimic
    for (int i = (*deck).num_cards - 1; i > 0; i--) // loops through all cards
    {
        j = rand() % (i + 1); // random index
        CARD temp = (*deck).cards[i];
        (*deck).cards[i] = (*deck).cards[j];
        (*deck).cards[j] = temp;
    }
}

void deal_cards(DECK *deck, HAND *players, BOARD *board) 
{
    for (int i = 0; i < 4; i++) //loop through players
    {
        for (int j = 0; j < 4; j++) //loop through the players cards
        {
            players[i].cards[j] = (*deck).cards[(*deck).num_cards - 1]; // assign the card
            (*deck).num_cards--;
        }
    }
    
    for (int j = 0; j < 4; j++) 
    {
        (*board).cards[j] = (*deck).cards[(*deck).num_cards - 1]; // assign the card
        (*deck).num_cards--;
    }
    
}

void display_card(CARD card) 
{
    if (card.value == 1) 
    {
        printf("Ace");
    } 
    else if (card.value == 11) 
    {
        printf("Jack");
    } 
    else if (card.value == 12) 
    {
        printf("Queen");
    } 
    else if (card.value == 13) 
    {
        printf("King");
    } 
    else 
    {
        printf("%d", card.value);
    }
    
    printf(" of ");
    
    switch (card.suit) 
    {
        case SPADES:
            printf("Spades");
            break;
        case HEARTS:
            printf("Hearts");
            break;
        case DIAMONDS:
            printf("Diamonds");
            break;
        case CLUBS:
            printf("Clubs");
            break;
    }
}

void display_hand(HAND hand) 
{
    for (int i = 0; i < 4; i++) 
    {
        display_card(hand.cards[i]);
        printf("\n");
    }
}

void display_board(BOARD board,int n) 
{
    printf("Board:\n");
    for (int i = 0; i < n; i++) 
    {
        display_card(board.cards[i]);
        printf("\n");
    }
}

int count_points(CARD collected_cards[], int num_collected) 
{
    int points = 0;
    int num_aces = 0;
    int num_jacks = 0;
    int num_twos = 0;
    int num_tens = 0;
    
    // find the number of each type of card
    for (int i = 0; i < num_collected; i++) 
    {
        switch (collected_cards[i].value) 
        {
            case 1: // ace
                num_aces++;
                break;
            case 11: // jack
                num_jacks++;
                break;
            case 2: // two of Clubs
                num_twos++;
                break;
            case 10: // ten of Diamonds
                num_tens++;
                break;
            default:
                break;
        }
    }
    
    points += num_aces; // aces is worth 1 point
    points += num_jacks; // Jacks is worth 1 point
    points += num_twos * 2; // two of Clubs is worth 2 points
    points += num_tens * 3; // ten of Diamonds is worth 3 points
    
    return points;
}

CARD recommendCard(BOARD board, HAND hand) 
{
    // the question said recommend the card at random 
    //so i just chose to recomment the 1st card on board
    return board.cards[0];
}

int bastra(BOARD *board, CARD collected_cards[], int num_collected) 
{
    // checking if the board is empty
    if ((*board).cards[0].value == 0) 
    {
        for (int i = 0; i < num_collected; i++) 
        {
            if (collected_cards[i].value == 11) 
            { // Jack
                return 0; // Jack found, Bastra not achieved
            }
        }
        return 10; // board is empty and no Jack found, Bastra achieved
    } 
    else 
    {
        return 0; // board is not empty, Bastra not achieved
    }
}

int next_player(int current_player) 
{
    return (current_player + 1) % 4;
}

void jack(BOARD *board, CARD collected_cards[], int *num_collected) 
{
    printf("You played a Jack! Taking all cards from the board.\n");
    for (int i = 0; i < 4; i++) 
    {
        if ((*board).cards[i].value != 0) 
        {
            collected_cards[*num_collected] = (*board).cards[i];
            (*num_collected)++;
            (*board).cards[i].value = 0;
        }
    }
}

void take_cards_from_board(BOARD *board, HAND *player, int chosen_hand_card_index, int *cards_on_board, CARD collected_cards[], int *num_collected, int current_player, int player_points[], int *board_filled) 
{
    printf("Enter the number of cards you want to take from the board: ");
    int num_cards_to_take;
    scanf("%d", &num_cards_to_take);
    
    if (num_cards_to_take < 0) 
    {
        printf("Invalid input.\n");
        return; 
    }
    
    if (num_cards_to_take > 0) {
        printf("Enter the index of the card(s) you want to take from the board (1-4): ");
        int chosen_board_cards[num_cards_to_take];
        for (int i = 0; i < num_cards_to_take; i++) 
        {
            scanf("%d", &chosen_board_cards[i]);
            if (chosen_board_cards[i] < 1 || chosen_board_cards[i] > *cards_on_board || (*board).cards[chosen_board_cards[i] - 1].value == 0) 
            {
                printf("Invalid input. Please enter a valid index corresponding to a card on the board.\n");
                i--;
            }
        }
        
        // Calculate the sum of the values of the chosen cards from the board
        int sum_chosen_board_cards = 0;
        for (int i = 0; i < num_cards_to_take; i++) 
        {
            sum_chosen_board_cards += (*board).cards[chosen_board_cards[i] - 1].value;
            (*cards_on_board)--;
        }

        
        // Check if the sum of the chosen cards from the board matches the value of the chosen card from the hand
        if (sum_chosen_board_cards == (*player).cards[chosen_hand_card_index - 1].value)
        {
            // Move the chosen cards from the board to the current player's collected cards
            for (int i = 0; i < num_cards_to_take; i++) 
            {
                collected_cards[*num_collected] = (*board).cards[chosen_board_cards[i] - 1];
                (*num_collected)++;
                (*board).cards[chosen_board_cards[i] - 1].value = 0;
            }
            
            // Move the chosen card from the hand to the collected cards
            collected_cards[*num_collected] = (*player).cards[chosen_hand_card_index - 1];
            (*num_collected)++;
            (*player).cards[chosen_hand_card_index - 1].value = 0;
            
            printf("Successfully collected cards.\n");
            
            *board_filled = 0; //
        } 
        else 
        {
            printf("The sum of the chosen cards from the board does not match the value of the chosen card from your hand.\n");
        }
    }
}

void place_card_on_board(BOARD *board, HAND *player, int chosen_hand_card_index, int *cards_on_board) 
{
    int chosen_card_index = chosen_hand_card_index - 1;
    // Find an empty place on the board to place the chosen card
    int empty_slot_index = -1;
    for (int i = 0; i < 20; i++) 
    { 
        if ((*board).cards[i].value == 0) 
        {
            empty_slot_index = i;
            break;
        }
    }
    if (empty_slot_index != -1) 
    {
        // the the chosen card from the player to the empty place on the board
        (*board).cards[empty_slot_index] = (*player).cards[chosen_card_index];
        printf("Added your chosen card to the board.\n");
        (*cards_on_board)++;
    } 
    else 
    {
        printf("No space available on the board to add your chosen card.\n");
    }
}

void refill_board(BOARD *board, DECK *deck) 
{
    // if the board is empty
    if ((*board).cards[0].value == 0) 
    {
        // Refill the board from the deck
        for (int i = 0; i < 4; i++) 
        {
            if ((*deck).num_cards > 0) 
            {
                (*board).cards[i] = (*deck).cards[(*deck).num_cards - 1];
                (*deck).num_cards--;
            } 
            else 
            {
                break; // If the deck is empty, stop filling the board
            }
        }
    }
}

int Mode() 
{
    int mode;
    printf("Choose gameplay mode:\n");
    printf("1. Play individually\n");
    printf("2. Play in pairs\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &mode);
    return mode;
}

int find_most_cards(HAND players[], int num_players) //looks for player with most cards
{
    int most_cards = 0;
    int most_cards_player = 0;
    int num_cards;
    for (int i = 0; i < num_players; i++) 
    {
        num_cards = 0;
        for (int j = 0; j < 4; j++) {
            if (players[i].cards[j].value != 0) 
            {
                num_cards++;
            }
        }
        if (num_cards > most_cards) 
        {
            most_cards = num_cards;
            most_cards_player = i;
        }
    }
    return most_cards_player;
}

int main() {
    DECK deck;
    HAND players[4];
    BOARD board;
    CARD collected_cards[16]; 
    int num_collected = 0;
    int current_player = 0;
    int player_points[4] = {0}; // array to store points earned by each player
    int board_filled = 0; // to track if the board is filled
    
    reset_game_state(&deck, players, &board);

    initial_deck(&deck);
    shuffle_deck(&deck);
    int mode=Mode();
    int most_cards_player;

    deal_cards(&deck, players, &board);
    int cards_on_board=4;

    while (1) //main game loop
    {
        // game state
        printf("Player %d's turn\n", current_player);
        display_hand(players[current_player]);
        display_board(board,cards_on_board);
        
        CARD recommended_card = recommendCard(board, players[current_player]);
        printf("Recommendation: ");
        display_card(recommended_card);
        printf("\n");
        
        // the current player to choose a card from their hand
        printf("Enter the index of the card you want to play from your hand (1-4): ");
        int chosen_hand_card_index;
        scanf("%d", &chosen_hand_card_index);
        
        if (chosen_hand_card_index < 1 || chosen_hand_card_index > 4 || players[current_player].cards[chosen_hand_card_index - 1].value == 0) 
        {
            printf("Invalid input. Please enter a valid index corresponding to a card in your hand.\n");
            continue;
        }
        
        
        // Check if the chosen card from the hand is a Jack
        if (players[current_player].cards[chosen_hand_card_index - 1].value == 11) {
            jack(&board, collected_cards, &num_collected);
        } else {
            // Ask the player if they want to take a card from the board
            printf("Do you want to take a card from the board? (1 for Yes, 0 for No): ");
            int take_card_option;
            scanf("%d", &take_card_option);

            if (take_card_option == 1) {
                take_cards_from_board(&board, &players[current_player], chosen_hand_card_index, &cards_on_board, collected_cards, &num_collected, current_player, player_points, &board_filled);
            }

            if (take_card_option == 0) {
                place_card_on_board(&board, &players[current_player], chosen_hand_card_index, &cards_on_board);
            }
        }
        if (mode==1)
        {
        // Count points for collected cards
        int points = count_points(collected_cards, num_collected);
        player_points[current_player] += points;
        printf("Points earned in this hand: %d\n", points);

        // Display points earned by each player
        printf("Points after this hand:\n");
        for (int i = 0; i < 4; i++) 
        {
            printf("Player %d: %d\n", i, player_points[i]);
        }
        // add extra points for the player with the most cards
        if (current_player == 4)
        {
            most_cards_player = find_most_cards(players, 4);
            player_points[most_cards_player] += 10;
        }
        }
        else if (mode == 2) 
        {
            int team_1_score = 0;
            int team_2_score = 0;

            // Determine which players are in each team
            int team_1[] = {0, 2}; // Players 0 and 2 in team 1
            int team_2[] = {1, 3}; // Players 1 and 3 in team 2
            
            // Calculate points for each team based on collected cards
            for (int i = 0; i < 4; i++) 
            {
                int points = count_points(collected_cards, num_collected);
                if (i == team_1[0] || i == team_1[1]) 
                {
                    team_1_score += points;
                } 
                else 
                {
                    team_2_score += points;
                }
            }
            if (current_player == 4)
            {
                most_cards_player = find_most_cards(players, 4);
                player_points[most_cards_player] += 10;
            }

            // Assign points to each team
            printf("Points for Team 1: %d\n", team_1_score);
            printf("Points for Team 2: %d\n", team_2_score);
        }

        // Check if the board is filled
        if (cards_on_board == 0)
        {
            cards_on_board = 4;  
        }
        refill_board(&board, &deck);
        
        // Check for Bastra
        int bastra_points = bastra(&board, collected_cards, num_collected);
        if (bastra_points == 10) 
        {
            player_points[current_player] += 10;
            printf("Bastra achieved by Player %d! They receive 10 points.\n", current_player);
        } 
        else 
        {
            printf("No Bastra achieved by Player %d.\n", current_player);
        }

        // display points after Bastra check
        printf("Points after Bastra:\n");
        for (int i = 0; i < 4; i++) 
        {
            printf("Player %d: %d\n", i, player_points[i]);
        }
        // determine the next player
        current_player = next_player(current_player);
    }
    
    return 0;
}

