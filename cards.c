#include "cards.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_RANKS 13
#define NUM_SUITS 4

char* deal(int deckSize, struct Cards* deck){
  srand(time(NULL));
  int randIndex = rand() % deckSize;
  char* card = malloc(strlen(deck->cards[randIndex]) + 1);;
  strncpy(card, deck->cards[randIndex], strlen(deck->cards[randIndex]) + 1);
  for(int i = randIndex; i < deckSize - 1; i++){
    deck->cards[i] = deck->cards[i + 1];
  }
  return card;
}


/**
  *   Function that creates a new deck of card.
  *   @return deck - Pointer to the newly created deck.
  */
struct Cards createDeck(){
  const char* ranks[NUM_RANKS] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
  const char* suits[NUM_SUITS] = {"S", "H", "D", "C"};
  struct Cards newDeck;
  char buffer[4];

  
  int mainDeckIndex = 0;
  for(int i = 0; i < NUM_SUITS; i++){
    for(int j = 0; j < NUM_RANKS; j++){
      buffer[0] = '\0';
      strncpy(buffer, ranks[j], sizeof(buffer) - 1);
      strncat(buffer, suits[i], sizeof(buffer) - strlen(buffer) - 1);
      newDeck.cards[mainDeckIndex++] = strndup(buffer, sizeof(buffer));
    }
  }
  return newDeck;
}

struct Cards shuffle(){
  struct Cards deck = createDeck();
  return deck;
}

void printDeck(struct Cards* deck, int deckSize){
  char startingSuit = deck->cards[0][1]; 
  for(int i = 0; i <  deckSize; ++i){
    char newSuit = deck->cards[i][strlen(deck->cards[i])-1];
    if(startingSuit != newSuit){
      printf("\n");
      startingSuit = deck->cards[i][1];
    }
      printf("%s ", deck->cards[i]);
    }
}

