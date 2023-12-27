struct Cards{
  const char* cards[52];
};

struct Cards shuffle();
struct Cards createDeck();
char* deal(int deckSize, struct Cards* deck);
void printDeck(struct Cards* deck, int deckSize);