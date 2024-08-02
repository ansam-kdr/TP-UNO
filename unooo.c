#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <time.h>
#include "uno_machine_abstraite.h"

char colors[4][8]={"Red","Green","Yellow","Bleu"}; 
char values[13][10]={"0","1","2","3","4","5","6","7","8","9","Reverse","Skip","Draw two"};

// Define color codes for each color
    #define ANSI_COLOR_RED     "\x1b[31m"
    #define ANSI_COLOR_BLUE    "\x1b[34m"
    #define ANSI_COLOR_YELLOW  "\x1b[33m"
    #define ANSI_COLOR_GREEN   "\x1b[32m"
    #define ANSI_COLOR_RESET   "\x1b[0m"


pointer buildDeck()
{
    pointer head,iter,newcard;
    allocate_card(&head);
   
    char* r="red";
    char* o="0";
    aff_data(head,r,o);
    aff_adr(head,NULL);
    iter=head;
    
    for(int i=0;i<=3;i++)
    {
        for(int j=0;j<=12;j++)
        {
            allocate_card(&newcard);
            char* c=colors[i];
            char* v=values[j];
            aff_data(newcard,c,v);
            aff_adr(iter,newcard);
            iter=next_card(iter);
        }
    }
    char* red="red";
    char* zero="0";
    deleteCard(&head,red,zero);
    return (head);
}

void shuffleDeck(pointer head) {
    // Count the number of nodes in the linked list
    int count = 0;
    pointer temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    // Seed the random number generator
    srand(time(NULL));

    // Perform the Fisher-Yates shuffle
    for (int i = 0; i < count; i++) {
        // Generate a random index
        int j = i + rand() % (count - i);

        // Swap the values at indices i and j
        pointer node_i = head;
        for (int k = 0; k < i; k++) {
            node_i = node_i->next;
        }

        pointer node_j = head;
        for (int k = 0; k < j; k++) {
            node_j = node_j->next;
        }

        char* temp_color = node_i->color;
        char* temp_value = node_i->value;

        node_i->color = node_j->color;
        node_i->value = node_j->value;

        node_j->color = temp_color;
        node_j->value = temp_value;
    }
}

pointer drawCards(int numcards, pointer* head) {
    if (*head == NULL) return NULL;  // No cards to draw

    pointer cardsDrawn = NULL, lastDrawn = NULL;

    for (int i = 0; i < numcards; i++) {
        if (*head == NULL) break;  // No more cards to draw

        // Create a new card and copy the data from the deck
        pointer newcard;
        allocate_card(&newcard);
        char* c = (*head)->color;
        char* v = (*head)->value;
        aff_data(newcard, c, v);

        // Add the new card to the drawn cards list
        if (cardsDrawn == NULL) {
            cardsDrawn = newcard;  // First card drawn
        } else {
            aff_adr(lastDrawn, newcard);  // Subsequent cards drawn
        }
        lastDrawn = newcard;

        // Remove the card from the deck
        pointer temp = *head;
        *head = next_card(*head);
        // Don't call free_card here, because the card is still in use
    }

    // Make sure the last drawn card doesn't link back to the deck
    if (lastDrawn != NULL) {
        aff_adr(lastDrawn, NULL);
    }

    return cardsDrawn;
}

ptrPlayer dealCards(int numcards, int numplayers, pointer* head) {
    if (*head == NULL) return NULL;  // No cards to deal

    ptrPlayer head_players = NULL, lastPlayer = NULL;

    for (int i = 0; i < numplayers; i++) {
        // Allocate a new player and deal cards to the player
        ptrPlayer newplayer;
        allocate_player(&newplayer);
        aff_hand(newplayer, drawCards(numcards, head));

        // Add the new player to the list of players
        if (head_players == NULL) {
            head_players = newplayer;  // First player
        } else {
            aff_next_player(lastPlayer, newplayer);
            aff_prev_player(newplayer, lastPlayer);
        }
        lastPlayer = newplayer;
    }

    // Make sure the last player doesn't link to a non-existent player
    aff_next_player(lastPlayer, NULL);

    return head_players;
}

void showHand(int player, ptrPlayer head_players) {
    printf("\nPlayer %d\nYour hand:\n", player + 1);

    ptrPlayer iterP = head_players;
    for (int i = 0; i < player; i++) {
        iterP = next_player(iterP);
    }

    pointer iter = iterP->hand;
    int x = 1;

     for (int i = 0; i < length(iterP->hand); i++) {
        if (strcmp(iter->color, "Red") == 0) {
            printf(ANSI_COLOR_RED "%d) %s %s\t" ANSI_COLOR_RESET, x, iter->color, iter->value);
        } else if (strcmp(iter->color, "Green") == 0) {
            printf(ANSI_COLOR_GREEN "%d) %s %s\t" ANSI_COLOR_RESET, x, iter->color, iter->value);
        } else if (strcmp(iter->color, "Yellow") == 0) {
            printf(ANSI_COLOR_YELLOW "%d) %s %s\t" ANSI_COLOR_RESET, x, iter->color, iter->value);
        } else {
            printf(ANSI_COLOR_BLUE "%d) %s %s\t" ANSI_COLOR_RESET, x, iter->color, iter->value);
        }
        iter = next_card(iter);
        x+=1;
    }

    printf("\n");
    return;
}


pointer discards(pointer* head_ref) {
    if (*head_ref == NULL) {
        return NULL;
    }

    // Allocate a new node for the discard pile
    pointer discard;
    allocate_card(&discard);

    // Copy the data from the head of the list to the discard node
    char* c=(*head_ref)->color;
    char* v=(*head_ref)->value;
    aff_data(discard, c, v);
    aff_adr(discard, NULL);

    // Store the old head node
    pointer temp = *head_ref;

    // Move the head node to the next node
    *head_ref = next_card(*head_ref);

    // Free the old head node
    free_card(&temp);

    // Return the new discard node
    return discard;
}

int canPlay(int player, ptrPlayer head_players, pointer discardPile) {
    ptrPlayer iterP = head_players;
    pointer Pile=getLastCard(discardPile);
    for (int i = 0; i < player; i++) {
        iterP = next_player(iterP);
    }
    pointer iter = iterP->hand;
    while (iter != NULL && (strcmp(iter->color, Pile->color) != 0 && strcmp(iter->value, Pile->value) != 0)) {
        iter = next_card(iter);
    }
    if (iter == NULL) {
        return 1;
    } else {
        return 0;
    }
}

int canPlayCard(int cardnum, ptrPlayer player, pointer discardPile) {
    pointer iter = player->hand;
    pointer Pile=getLastCard(discardPile);
    for (int i = 0; i < cardnum; i++) {
        iter = next_card(iter);
    }
    if (iter != NULL && (strcmp(iter->color, Pile->color) == 0 || strcmp(iter->value, Pile->value) == 0)) {
        return 0;
    } else {
        return 1;
    }
}

ptrPlayer pointerplayer(int player,ptrPlayer head_players)
{
ptrPlayer iterP=head_players;
    for (int i = 0; i < player; i++)
    {
        iterP=next_player(iterP);
    }
    return (iterP);
}

void concatenate(pointer a, pointer b) {
    if (a == NULL || b == NULL) {
        return;
    }

    // Traverse the first list until the end
    while (a->next != NULL) {
        a = a->next;
    }

    // Link the last node of the first list to the first node of the second list
    a->next = b;
}

pointer pointercard(int cardnum,pointer deck)
{
    pointer iter=deck;
    for (int i = 0; i < cardnum; i++)
    {
        iter=next_card(iter);
    }
    return (iter);

}
void playCard(ptrPlayer player,int cardnum,pointer *discardpile,pointer deck)
{
    pointer discarded;
    pointer ptrcard=pointercard(cardnum,player->hand);
    allocate_card(&discarded);
    aff_data(discarded,ptrcard->color, ptrcard->value);
    aff_adr(discarded, NULL);
    aff_adr(*discardpile,discarded);
    char* c=ptrcard->color;
    char* v=ptrcard->value;
    deleteCard(&(player->hand),c,v);
}


int main()
{
    pointer deck=buildDeck();
    shuffleDeck(deck);

    pointer discardPile=discards(&deck);
    printf("\n");
    int incrementPlayersTurn = 0;
    int playersTurn=0;  //0 refers to player 1
    int playing = 0;
    int playDirection=1;
    int winner=1;
    printf(" \n Welcom To UNO_CONSOLE \n");
    int numPlayers=0;
    while (numPlayers<2 || numPlayers>4)
    {
        printf("Enter the number of players (2 to 4):");
        scanf("%d",&numPlayers);
    }
    
    printf("\n How many cards would you like to draw at the begining?");
    int numCards=0;
    scanf("%d",&numCards);
    
    printf("Let's Go");
    ptrPlayer headPlayers=dealCards(numCards,numPlayers,&deck);
    
    while (playing==0)
    {
        printf("\nThe card on to of the discard pile is:  ");
        printf("%s %s \n", getLastCard(discardPile)->color,getLastCard(discardPile)->value);
        
        showHand(playersTurn,headPlayers);
        
        if (canPlay(playersTurn,headPlayers,discardPile)==0)
        {
            printf("Player can play a card. Asking for choice...\n");
            int choice=0;
            while(choice!=1 && choice!=2)
            {
            printf("Play or Draw?\n Enter 1 for 'Play' and 2 for 'Draw'");
            scanf("%d",&choice);
            }
            if(choice==1)
            {
            int cardnum = -1;
            while (cardnum < 0 || cardnum >= length((pointerplayer(playersTurn,headPlayers))->hand) || canPlayCard(cardnum, pointerplayer(playersTurn,headPlayers), discardPile) != 0) {
                printf("Which card do you want to play?");
                scanf("%d", &cardnum);
                cardnum--; // Adjust for 1-indexing if necessary
            }

            playCard(pointerplayer(playersTurn,headPlayers), cardnum, &discardPile, deck);
            printf("You played: %s %s\n", (getLastCard(discardPile))->color, (getLastCard(discardPile))->value);

            
                //check if the player won
                if (length(pointerplayer(playersTurn,headPlayers)->hand)==0)
                {
                    playing=1;
                    winner=playersTurn+1;
                }
               else
               {
                    // check for special cards
                    // Reverse works
                    if (strcmp(getLastCard(discardPile)->value, "Reverse") == 0) // Use 'strcmp' to compare strings
                    {
                    playDirection=playDirection*(-1);
                    printf("playDirection= %d",playDirection);
                
                    }
                    //skip works
                    else if (strcmp(getLastCard(discardPile)->value, "Skip") == 0) 
                    {
                        playersTurn+=playDirection;
                        if (playersTurn>=numPlayers)
                        {
                            playersTurn=0;
                        }
                        else if (playersTurn<0)
                        {
                            playersTurn=numPlayers-1;
                        }
                    } 
                    // draw two works
                    else if (strcmp(getLastCard(discardPile)->value, "Draw two") == 0) 
                    {
                        int playerDraw=playersTurn+playDirection;
                        if (playerDraw==numPlayers)
                        {
                            playerDraw=0;
                        }
                        else if (playerDraw<0)
                        {
                            playerDraw=numPlayers-1;
                        }
                        concatenate(pointerplayer(playerDraw,headPlayers)->hand,drawCards(2,&deck));
                        
                    }    
                }
            }
        else {
                concatenate(pointerplayer(playersTurn,headPlayers)->hand,drawCards(1,&deck));
            }
        }
        else
        {
            printf("you can't play; you need to draw a card");
            concatenate(pointerplayer(playersTurn,headPlayers)->hand,drawCards(1,&deck));
            showHand(playersTurn,headPlayers);
            printf("\n");
        }
        if (incrementPlayersTurn == 0)
        {
        playersTurn+=playDirection;
        if (playersTurn>=numPlayers)
        {
            playersTurn=0;
        }
        else if (playersTurn<0)
        {
            playersTurn=numPlayers-1;
        }
   }
    }
    printf("GAME OVER`\n");
    printf("The winner is player %d!",winner);
    freeCardList(deck);
    freeCardList(discardPile);
    freePlayerList(headPlayers);
    return 0;
}
