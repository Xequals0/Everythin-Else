// This is the main program for the blackjack game.
public class Blackjack
{
    
    // 	- Play rounds until the players want to quit the game.
    //	- Close the window.
    public static void main(String[] args)
    {
        int numPlayers; //Used to set the number of players for the game.
        boolean run = true; // run condition, used to decide weather or not to begin the main loop in the game.
        BlackjackWindow w = null;
        System.out.println("Welcome...Let's Play BlackJack!");
        
        do{ // enter a valid number of player's: 1-3; hold this value in numPlayers.
            System.out.println("Please enter the number of players (up to 3 players): ");
            numPlayers = IO.readInt();
            if(numPlayers <= 0 || numPlayers > 3)
                System.out.println("Try again, Enter a number between 1 and 3 (inclusive)");
        }while(numPlayers <= 0 || numPlayers > 3);
        
        Player[] players = new Player[numPlayers + 1];
        
        
        for(int i = 0; i < numPlayers + 1; i++){
            
            System.out.println("Name of player "+ (i + 1) +" :" );
            String name = IO.readString();
            System.out.println("Is player " + (i + 1) + " the dealer?    Y/N");
            boolean dealer = IO.readBoolean();
            
            players[i] = new Player(name,dealer);
        }
        while(run){ // if at least one player is not bankrupt and the players wish to play (again), rut the main game loop.
            
            System.out.println("Beginning Round..."+ "\n" + "Dealer shuflles the Deck");
            w = new BlackjackWindow(players);
            playRound(players, w);
            run = GIO.readBoolean("Do you want to play again? ('y' for yes, 'n' for no)");
            // asks the players if they wish to play another round.
            if(!run){
                break;
            }
            else continue;
        } GIO.displayMessage("Have a nice day!!!");
        w.close();
    }
    
    // This method executes an single round of play (for all players).  It should:
    //	- Create and shuffle a deck of cards.
    //	- Start the round (deal cards) for each player, then the dealer.
    //	- Allow each player to play, then the dealer.
    //	- Finish the round (announce results) for each player.
    public static void playRound(Player[] players, BlackjackWindow window)
    {
        Deck dec1 = new Deck();
        dec1.shuffle();
        for(int i=0; i<players.length;i++){
            if(players[i].isDealer() == true){
                Player temp = players[i];
                players[i] = players[players.length-1];
                players[players.length-1] = temp;
                break;
            }
        } // puts dealer as last player. search and replace.
        
        
        for(int i = 0; i<players.length; i++){
            players[i].startRound(dec1, window);
        }
        //players + dealer start round. Card dealt.
        
        for(int i = 0; i<players.length; i++){
            if(players[i].isDealer() == false)
                players[i].playRound(dec1, window);
        }
        //players + dealer play round. Hit or stand.
        
        int dealerhand = 0;
        for(int i = 0; i<players.length; i++){
            if(players[i].isDealer() == true){
                dealerhand = players[i].getHand().getScore();
            }
        }
        //get dealer's hand value.
        for(int i = 0; i<players.length; i++){
            players[i].finishRound(dealerhand, window);
        }
        
        
    }// end of playRound
}// end of Blackjack class
