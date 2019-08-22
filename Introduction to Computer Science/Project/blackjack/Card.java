public class Card
{
	// Card suits (provided for your convenience - use is optional)
	public static final int SPADES   = 0;
	public static final int HEARTS   = 1;
	public static final int CLUBS    = 2;
	public static final int DIAMONDS = 3;

	// Card faces (provided for your convenience - use is optional)
	public static final int ACE      = 1;
	public static final int TWO      = 2;
	public static final int THREE    = 3;
	public static final int FOUR     = 4;
	public static final int FIVE     = 5;
	public static final int SIX      = 6;
	public static final int SEVEN    = 7;
	public static final int EIGHT    = 8;
	public static final int NINE     = 9;
	public static final int TEN      = 10;
	public static final int JACK     = 11;
	public static final int QUEEN    = 12;
	public static final int KING     = 13;

	//define fields.
	private int suit;
	private int face;
	private boolean faceup; // used to turn card face-up/down

	// This constructor builds a card with the given suit and face, turned face down.
	public Card(int cardSuit, int cardFace)
	{
		suit = cardSuit;
		face = cardFace;
		faceup = false;
	}
	// This method retrieves the suit (spades, hearts, etc.) of this card.
	public int getSuit()
	{
		return suit; 
	}	
	// This method retrieves the face (ace through king) of this card.
	public int getFace()
	{
		return face;
	}	
	
	// This method retrieves the numerical value of this card
	// (usually same as card face, except 1 for ace and 10 for jack/queen/king)
	public int getValue()
	{
		int value;
		if(face >= 10 )
			value = 10;
		else 
			value = this.face;	
		return value;
	}
	
	// This method determines whether the front of the card should be visible.
	public boolean isFaceUp()
	{ //returns the boolean
		return faceup; 
	}
	
	// This method records that the front of the card should be visible.
	public void turnFaceUp()
	{
		faceup = true;
	}
	
	// This method records that only the back of the card should be visible.
	public void turnFaceDown()
	{
		faceup = false;
	}
	
	public String toString(){
		String cardname = "";
				
		String suitname = "";
		if(suit == 0)
			suitname = "Spades";
		if(suit ==1)
			suitname = "Hearts";
		if(suit ==2)
			suitname = "Clubs";
		if(suit ==3)
			suitname = "Diamonds";
		
		if(this.face == 1 || this.face > 10){
			String facename = "";
			if(face == 1)
				facename = "Ace";
			if(face == 11)
				facename = "Jack";
			if(face == 12)
				facename = "Queen";
			if(face == 13)
				facename = "King";
			
			cardname = facename + " of " + suitname;
		}		
		else if(face != 1 && face <= 10)
			cardname = face + " of " + suitname;			
		return cardname;
	}
}