#include <stdio.h>
#include <stdlib.h>

typedef struct card *pointer;
typedef struct Player *ptrPlayer;

struct card
{
    char* color; 
    char* value;
    pointer next;
};

struct Player 
{
    pointer hand;
    ptrPlayer next;
    ptrPlayer previous;
};
void allocate_card(pointer *head)
{
    *head = (struct card*) malloc(sizeof(struct card));
}
void free_card(pointer *head){
    free(*head);                                           
}
pointer next_card(pointer head){
    return(head->next);
}
char* data_card(pointer head){
    return(head->color,head->value);
}
void aff_data(pointer p,char* c,char* v){
    p->color=c;
    p->value=v;
}
void aff_adr (pointer p,pointer q){
    p->next=q;
}


int length(pointer head){
    int length=0;
   
    pointer iter=head;
    if ( head==NULL){
       return(0);
    }
    else{
        while (iter != NULL)
        {
            length ++;
            iter = next_card(iter);
        }
        
    }
    return(length) ;
}
void allocate_player(ptrPlayer *head)
{
    *head=(struct Player*)  malloc(sizeof(struct Player));
}
ptrPlayer  next_player(ptrPlayer head)
{
    return(head->next);
}
void aff_hand(ptrPlayer p,pointer hand)
{
    p->hand=hand;
}
void aff_next_player(ptrPlayer p,ptrPlayer q)
{
    p->next=q;
}
void aff_prev_player(ptrPlayer p,ptrPlayer q)
{
    p->previous=q;
}
void freePlayer(ptrPlayer *head)
{
    free(*head);
}
int lengthP(ptrPlayer head){
    int length=0;
   
    ptrPlayer iter=head;
    if ( head==NULL){
       return(0);
    }
    else{
        while (iter != NULL)
        {
            length ++;
            iter = next_player(iter);
        }
        
    }
    return(length) ;
}
void deleteCard(pointer* head, char* color, char* value) {
    // If the head node itself holds the key to be deleted
    if (*head != NULL && strcmp((*head)->color, color) == 0 && strcmp((*head)->value, value) == 0) {
        pointer temp = *head;
        *head = (*head)->next;
        free(temp);
        return;
    }

    // Search for the key to be deleted, keep track of the previous node
    pointer temp = *head, prev;
    while (temp != NULL && (strcmp(temp->color, color) != 0 || strcmp(temp->value, value) != 0)) {
        prev = temp;
        temp = temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL) return;

    // Unlink the node from linked list
    prev->next = temp->next;

    free(temp);  // Free memory
}
pointer getLastCard(pointer discardPile) {
    // If the discard pile is empty, there's no last card
    if (discardPile == NULL) return NULL;

    // Traverse the discard pile to the last card
    pointer iter = discardPile;
    while (iter->next != NULL) {
        iter = iter->next;
    }

    // Return a pointer to the last card
    return iter;
}
void freeCardList(pointer head) {
    pointer temp;

    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
void freePlayerList(ptrPlayer head) {
    ptrPlayer temp;

    while (head != NULL) {
        freeCardList(head->hand);  // Free the player's hand of cards
        temp = head;
        head = head->next;
        free(temp);
    }
}
void print_cards(pointer head) {
    while (head != NULL) {
        printf(" %s %s ", head->color,head->value);
        head = head->next;
    }
}


