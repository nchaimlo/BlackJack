#include <stdio.h>
#include <stdlib.h>
#include "cards.h"

/**
* In the rare instance that the smallest possible card is given for each hit for a 
* single deck of cards, then the maximum number of cards that a player can 
* accumulate before breaking is 12.
* AH + AS + AD + AC + 2H + 2S + 2D + 2C + 3H + 3S + 3D + 3C = 24
*/
#define MAX_CARDS 12
#define MAX_HAND 21

void firstDeal(struct Cards* deck, int deckSize, char** playerHand, char** dealerHand){
  char* card = deal(deckSize, deck);
  for(int i = 0; i < 2; ++i){
    dealerHand[i] = strndup(deal(deckSize, deck),sizeof(dealerHand));
    --deckSize;
    playerHand[i] = strndup(deal(deckSize, deck), sizeof(playerHand));
    --deckSize;
  }
}

void hit(struct Cards* deck, int deckSize, char** hand, int handSize){
  hand[handSize] = strndup(deal(deckSize, deck), sizeof(hand));
}

int isGameOver(int playerPoints){
  if(playerPoints <= 0){
    return 0;
  }
  return -1;
}

int calculateCards(char** hand, int numCards){
  int total = 0;
  int numAces = 0;

  for(int i = 0; i < numCards; ++i){
    if(hand[i][0] == 'A'){
      total += 11;
      ++numAces;
    }
    else if(hand[i][0] == 'J' || hand[i][0] == 'Q' || hand[i][0] == 'K'){
      total += 10;
    }
    else{
      total += atoi(hand[i]);
    }
  }
  while(total > MAX_HAND && numAces > 0){
    total -= 10;
    --numAces;
  }
  return total;
}


int hasBlackJack(char** hand){
  if(calculateCards(hand, 2) == 21){
    return 0;
  }
  return -1;
}

void printHands(char** playerHand, char** dealerHand, int playerNumCards, int dealerNumCards, int isPlayersTurn){
  printf("Dealer: ");
  for(int i = 0; i < dealerNumCards; ++i){
    if(isPlayersTurn == 0 && i == 1){
      printf("?? ");
    }else{
      printf("%s ", dealerHand[i]);
    }
  }

  printf("\n");

  printf("Player: ");
  for(int i = 0; i < playerNumCards; ++i){
    printf("%s ", playerHand[i]);
  }
  printf("\n");
}



void printGameInfo(int playerPoints, int deckSize){
  printf("Total: %d\n", playerPoints);
  printf("Deck: %d\n", deckSize);
}

void playGame(struct Cards* deck, int* deckSize, char** playerHand, char** dealerHand, int* playerPoints){
  printf("*******Blackjack game*******\n");
  firstDeal(deck, *deckSize, playerHand, dealerHand);
  *deckSize -= 4;
  int playerNumCards = 2;
  int dealerNumCards = 2;
  

  if(hasBlackJack(playerHand) == 0){
    printHands(playerHand, dealerHand, playerNumCards, dealerNumCards, -1);
    if(hasBlackJack(dealerHand) == 0){
      printf("Push!\n");
      return;
    }else{
      printf("Player has blackJack!\n");
      *playerPoints += 10;
      return;
    }
  }

  
  char choice[6];
  while(1){
    printHands(playerHand, dealerHand, playerNumCards, dealerNumCards, 0);
    scanf("%5s", choice);
    if(strncmp(choice, "hit", sizeof("hit") - 1) == 0){
      hit(deck, *deckSize, playerHand, playerNumCards);
      ++playerNumCards;
      *deckSize -= 1;
      if(calculateCards(playerHand, playerNumCards) > 21){
        printHands(playerHand, dealerHand, playerNumCards, dealerNumCards, -1);
        printf("Player breaks! Dealer wins :(\n");
        *playerPoints -= 10;
        return;
      }
      
    }else if(strncmp(choice, "stay", sizeof("stand") - 1) == 0){
      printHands(playerHand, dealerHand, playerNumCards, dealerNumCards, -1);
      while(calculateCards(dealerHand, dealerNumCards) < 16){
        hit(deck, *deckSize, dealerHand, dealerNumCards);
        *deckSize -= 1;
        ++dealerNumCards;
        printHands(playerHand, dealerHand, playerNumCards, dealerNumCards, -1);
        printf("Dealer hit\n");
      }
      if(calculateCards(dealerHand, dealerNumCards) > 21){
        printf("Dealer Breaks! Player wins!\n");
        *playerPoints += 10;
        return;
      }else if(calculateCards(dealerHand, dealerNumCards) == calculateCards(playerHand, playerNumCards)){
        printf("Push!\n");
        return;
      }else if(calculateCards(dealerHand, dealerNumCards) > calculateCards(playerHand, playerNumCards)){
        printf("Dealer wins :(\n");
        *playerPoints -= 10;
        return;
      }else{
        printf("Player wins!\n");
        *playerPoints += 10;
        return;
      }
    }
  }
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(void) {
  int deckSize = 52;
  int playerPoints = 100;

  char* dealerHand[MAX_CARDS];
  char* playerHand[MAX_CARDS];

  
  struct Cards deck = createDeck();
  
  char input[10]; 

  while(1){
    printGameInfo(playerPoints, deckSize);
    if(deckSize < 20){
      deck = createDeck();
      printf("New deck in use!\n");
    }
    if(isGameOver(playerPoints) == 0){
      printf("Game over! Player has %d points.\n", playerPoints);
      break;
    }

    printf("Press space and enter to continue or enter 'quit' to exit: ");
    if (scanf("%9[^\n]", input) != 1) {
      while (getchar() != '\n');
      printf("Invalid input. Please try again.\n");
      continue;
    }

    if (strncmp(input, "quit", sizeof("quit") - 1) == 0) {
      printf("Exiting the game.\n");
      break;
    }
    
    playGame(&deck, &deckSize, playerHand, dealerHand, &playerPoints);
    clearInputBuffer();

    
  }
  return 0;
}

