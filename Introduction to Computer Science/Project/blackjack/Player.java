// This class represents one blackjack player (or the dealer)
public class Player
{
	// define fields here
	private String name;
	private boolean isDealer;
	private Hand h;

	// This constructor creates a player.
	// If isDealer is true, this Player object represents the dealer.
	public Player(String playerName, boolean isDealer)
		{
			name = playerName;
			this.isDealer = isDealer;
			h = new Hand();
		}
	// This method retrieves the player's name.
	public String getName()
	{
		return name; // replace this line with your code
	}
	// This method retrieves the player's dealing status
	public boolean isDealer()
	{
		return this.isDealer; 
	}
	
	// This method retrieves the player's hand of cards.
	public Hand getHand()
	{
		return h; // replace this line with your code
	}
	
	// This method deals two cards to the player (one face down if this is the dealer).
	// The window input should be used to redraw the window whenever a card is dealt.
	public void startRound(Deck deck, BlackjackWindow window)
	{
		if(this.isDealer == true){
			Card c1 = deck.deal();
			c1.turnFaceUp();
			Card c2 = deck.deal();
			c2.turnFaceDown();
			h.addCard(c1); 
			window.redraw();
			h.addCard(c2); 
			window.redraw();
			
			GIO.displayMessage("Dealer: " + name);
			
		}else if(this.isDealer == false){				
			Card c1 = deck.deal();
			Card c2 = deck.deal();
			c1.turnFaceUp();
			c2.turnFaceUp();
			h.addCard(c1); 
			window.redraw();
			h.addCard(c2); 
			window.redraw();
			System.out.println();			
		}
	
	}
	
	// This method executes gameplay for one player.
	// If this player is the dealer:
	//	- hits until score is at least 17
	// If this is an ordinary player:
	//	- repeatedly asks the user if they want to hit (draw another card)
	//	  until either the player wants to stand (not take any more cards) or
	//	  his/her score exceeds 21 (busts).
	// The window input should be used to redraw the window whenever a card is dealt or turned over.
	public void playRound(Deck deck, BlackjackWindow window)
	{
		int totalscore = h.getScore();
		GIO.displayMessage(name + " play round:");
		//System.out.println(h.toStringPlayer());
		GIO.displayMessage("\t" + name + "'s current score: " + h.getScore());
		
		if(this.isDealer == false){
			boolean hit = GIO.readBoolean("Hit?  Y/N"); 
			
			while(hit == true){
				Card c = deck.deal();
				c.turnFaceUp();
				h.addCard(c);
				window.redraw();
				GIO.displayMessage("\t" + name + "'s current score: " + h.getScore());

				if(h.getScore() == 21){
					GIO.displayMessage( name + " has blackjack!");
					return;
				}

				if(h.getScore() > 21){
					GIO.displayMessage("You busted.");
					return;
				}

				hit = GIO.readBoolean("Hit?  Y/N"); 
			}
		}
		if(this.isDealer == true){
			GIO.displayMessage("Dealer's Turn...");
			while(totalscore < 17){
				Card c = deck.deal();;
				c.turnFaceUp();
				h.addCard(c);
				window.redraw();
				GIO.displayMessage("\t" + name + "'s current score: " + h.getScore());
				
				if(h.getScore() > 21)
					GIO.displayMessage("Dealer busts :)");
				if(h.getScore() == 21)
					GIO.displayMessage("Dealer has blackjack (:");
				if(h.getScore() >= 17)
					return;		
			}//end of loop	
		}//end of dealer's round
	}//end of Player round

	// This method informs the player about whether they won, lost, or pushed.
	// It also discards the player's cards to prepare for the next round.
	// The window input should be used to redraw the window after cards are discarded.
	public void finishRound(int dealerScore, BlackjackWindow window)
	{	
		if(this.isDealer == false){		
			System.out.print(name + ":  ");
				if(h.getScore() > 21){
					GIO.displayMessage("You lose because you busted!");			
				}		
				else if(dealerScore > 21){
					GIO.displayMessage("You win!  The Dealer busted!");
				}
				else if( h.getScore() < dealerScore){
					GIO.displayMessage("You lose because the Dealer beat you!");
				}
				else if( h.getScore() > dealerScore){
					GIO.displayMessage("You win!  You beat the Dealer!");
				}
				else if( h.getScore() == dealerScore){
					GIO.displayMessage("You pushed because you tied the Dealer.");
				}	
		}
		h.discardAll();
		window.redraw();
		return; 
	}
}