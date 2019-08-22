// This class represents the set of cards held by one player (or the dealer).
public class Hand
{
	// define fields.
	private Card[] hand;
	// This constructor builds a hand (with no cards, initially).
	public Hand()
	{
		this.hand = new Card [52];
		for(int i=0; i<hand.length;i++){
			hand[i] = null;
		}
		
	}
	
	// This method retrieves the size of this hand.
	public int getNumberOfCards()
	{
		int notempty = 0;
		for(int i=0; i<hand.length;i++){
			if(hand[i] != null){
				notempty++;
			}
		}
		return notempty; // replace this line with your code
	}

	// This method retrieves a particular card in this hand.  The card number is zero-based.
	public Card getCard(int x)
	{
		return hand[x]; 
	}

	// This method takes a card and places it into this hand.
	public void addCard(Card newcard)
	{
		for(int i=0; i<this.hand.length;i++){
			if(hand[i] == null){
				hand[i] = newcard;
				break;
			}
		}
	}

	
	// This method computes the score of this hand.
	public int getScore()
	{
		int total = 0;
		
		for(int i=0; i<hand.length;i++){
			if(hand[i] != null){
				total = total + hand[i].getValue();
			}
		}
		//normal total (ignoring aces).	
		boolean ace = false;
		for(int i=0; i<hand.length;i++){
			if( hand[i] != null){	
				if(hand[i].getFace() == 1){
					ace = true;
				}
			}
		}
		// boolean, locates aces				
		if(ace == true && total+10 <= 21){
			total = total+10;
		}
		
		return total; // replace this line with your code
	}
	// This methods discards all cards in this hand.
	public void discardAll()
	{		
		for(int i=0; i<hand.length; i++){
			hand[i] = null;
		}	
	}
}