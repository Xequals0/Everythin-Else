//import java.util.Random;
// This class represents the deck of cards from which cards are dealt to players.
public class Deck
{
	// define fields here
	private Card[] deck;
	private int cardsDealt;
	
	// This constructor builds a deck of 52 cards.
	public Deck()
	{
		//fill this method in
		deck = new Card[52];
		int i = 0;
		for(int suit = 0; suit <= 3; suit++){
			for(int face = 1; face <= 13; face++){
				deck[i] = new Card(suit,face);
				i++;
			}
		}
		cardsDealt = 0;
	}

	
	// This method takes the top card off the deck and returns it.
	public Card deal()
	{
		//fill this method in
		if(isEmpty())
			shuffle();
		cardsDealt++;
		return deck[cardsDealt - 1];
	}
	
	public int count(Card c){ // utilizing a HI-Opt 2 strategy
		int count = 0;
		int value = c.getValue();
		if(value == 2 || value == 3 || value == 6 || value == 7){
			count = 1;
		}
		if(value == 4 || value == 5){
			count = 2;
		}
		if(value == 8 || value == 9 || value == 1){
			count = 0;
		}
		if(value == 10)
			count = -2;
		return count;
	}
	// This method returns the number of cards left in the deck.
		public int getSize()
		{
			int notempty = 0;			
			for(int i=0; i<deck.length;i++){
				if(this.deck[i] != null){
					notempty++;
				}
			}		
			return notempty;
		}
		
	// this method returns true if there are no more cards to deal, false otherwise
	public boolean isEmpty()
	{
		//fill this method in
		if(cardsDealt == 52)
			return true;
		else return false;
	}
	
	//this method puts the deck int some random order
	public void shuffle(){
		//fill this method in
		int randomNum;
		for(int i = 1; i < 52; i++){
			randomNum = (int) (Math.random()*(51)); // obtain a random number within the acceptable range
			Card temp = deck[i];
			deck[i] = deck[randomNum];
			deck[randomNum] = temp;
		}
		cardsDealt = 0;
	}
	
	
	//this method returns a String representation of the Deck object
	public String toString(){
		String s = "";
		for(int i = 0; i < 52; i++){
			s += deck[i]+ "\n";	
		}
		return s;
	}
}
