package solitaire;

import java.io.IOException;
import java.util.Scanner;
import java.util.Random;
import java.util.NoSuchElementException;

/**
 * This class implements a simplified version of Bruce Schneier's Solitaire Encryption algorithm.
 * 
 * @author RU NB CS112: Anirudh Tunoori
 */
public class Solitaire {
	
	/**
	 * Circular linked list that is the deck of cards for encryption
	 */
	CardNode deckRear;
	
	/**
	 * Makes a shuffled deck of cards for encryption. The deck is stored in a circular
	 * linked list, whose last node is pointed to by the field deckRear
	 */
	public void makeDeck() {
		// start with an array of 1..28 for easy shuffling
		int[] cardValues = new int[28];
		// assign values from 1 to 28
		for (int i=0; i < cardValues.length; i++) {
			cardValues[i] = i+1;
		}
		
		// shuffle the cards
		Random randgen = new Random();
 	        for (int i = 0; i < cardValues.length; i++) {
	            int other = randgen.nextInt(28);
	            int temp = cardValues[i];
	            cardValues[i] = cardValues[other];
	            cardValues[other] = temp;
	        }
	     
	    // create a circular linked list from this deck and make deckRear point to its last node
	    CardNode cn = new CardNode();
	    cn.cardValue = cardValues[0];
	    cn.next = cn;
	    deckRear = cn;
	    for (int i=1; i < cardValues.length; i++) {
	    	cn = new CardNode();
	    	cn.cardValue = cardValues[i];
	    	cn.next = deckRear.next;
	    	deckRear.next = cn;
	    	deckRear = cn;
	    }
	}
	
	/**
	 * Makes a circular linked list deck out of values read from scanner.
	 */
	public void makeDeck(Scanner scanner) 
	throws IOException {
		CardNode cn = null;
		if (scanner.hasNextInt()) {
			cn = new CardNode();
		    cn.cardValue = scanner.nextInt();
		    cn.next = cn;
		    deckRear = cn;
		}
		while (scanner.hasNextInt()) {
			cn = new CardNode();
	    	cn.cardValue = scanner.nextInt();
	    	cn.next = deckRear.next;
	    	deckRear.next = cn;
	    	deckRear = cn;
		}
	}
	
	/**
	 * Implements Step 1 - Joker A - on the deck.
	 */
	void jokerA() {
		// COMPLETE THIS METHOD
		CardNode ptr = deckRear;
		while (ptr.cardValue != 27){
			ptr = ptr.next;
		}
		
		int temp = ptr.next.cardValue;
		ptr.next.cardValue = ptr.cardValue;
		ptr.cardValue = temp;
	}
	
	/**
	 * Implements Step 2 - Joker B - on the deck.
	 */
	void jokerB() {
	    // COMPLETE THIS METHOD
		CardNode ptr = deckRear;
		while (ptr.cardValue != 28){
			ptr = ptr.next;
		}
		
		//first swap
		int temp = ptr.next.cardValue;
		ptr.next.cardValue = ptr.cardValue;
		ptr.cardValue = temp;
		ptr = ptr.next;	//move pointer for second swap
		
		//second swap
		temp = ptr.next.cardValue;
		ptr.next.cardValue = ptr.cardValue;
		ptr.cardValue = temp;
	}
	
	/**
	 * Implements Step 3 - Triple Cut - on the deck.
	 */
	void tripleCut() {
		// COMPLETE THIS METHOD
		CardNode prev = deckRear.next;
		CardNode ptr = deckRear.next.next;
		//if there are no cards before the first joker, make the second joker the last card
		if(deckRear.next.cardValue == 27 || deckRear.next.cardValue == 28)
		{	for(;ptr != deckRear; ptr = ptr.next)
			{
				if (deckRear.cardValue == 27 || deckRear.cardValue == 28){
					return;
				}
				else if (ptr.cardValue == 27 || ptr.cardValue == 28){
					CardNode temp = ptr;
					CardNode after = ptr.next;
					deckRear = temp;
					deckRear.next = after;
					return;
				}
				else prev = prev.next;
			}
		}
		//if there are no cards after the second joker, make the first joker the first card
		else if (deckRear.cardValue == 27 || deckRear.cardValue == 28)
		{	ptr = deckRear.next;prev = deckRear;
			for(;ptr != deckRear; ptr = ptr.next)
			{
				if(deckRear.next.cardValue == 27 || deckRear.next.cardValue == 28)
					return;
				else if(ptr.cardValue == 27 || ptr.cardValue == 28)
				{
					CardNode front = deckRear.next;
					CardNode temp = ptr;
					CardNode before = prev;
					deckRear.next = front;
					deckRear = before;
					deckRear.next = temp;
					return;
				}
				else prev = prev.next;
			}
		}
		else	//triple cut
		{
			prev = deckRear;ptr = deckRear.next;
			while(ptr!=deckRear)
			{
				if(ptr.cardValue == 27 || ptr.cardValue == 28)
				{
					CardNode J1 = ptr;	//the first joker
					CardNode afterJ1 = ptr.next; //points to the card after the first joker
					while(afterJ1 != deckRear.next)
					{
						if(afterJ1.cardValue == 27 || afterJ1.cardValue == 28){
							CardNode J2 = afterJ1; //J2 points to the second joker
							CardNode afterJ2 = afterJ1.next;
							CardNode front = deckRear.next;
							deckRear.next = J1;
							J2.next = front;
							deckRear = prev;
							deckRear.next = afterJ2;
							return;
						}
						else afterJ1 = afterJ1.next;
					}
				}	else {prev = prev.next; ptr = ptr.next;}
			}
		}
	}
	
	
	/**
	 * Implements Step 4 - Count Cut - on the deck.
	 */
	void countCut() {		
		// COMPLETE THIS METHOD
		CardNode secondToLast = null, ptr = deckRear.next;
		
		int count = deckRear.cardValue;
		if(count == 28 || count ==27){
			count = 27;
			return;
		}
		while(ptr.next.cardValue != deckRear.cardValue){
			secondToLast = ptr;
			ptr = ptr.next;
		}secondToLast = secondToLast.next;
		ptr = deckRear.next;
		for(int i = 0; i < (count -1); i++){
			ptr = ptr.next;
		}
		CardNode stop = ptr; CardNode start = ptr.next;
		
		secondToLast.next = deckRear.next;
		stop.next = deckRear;
		deckRear.next = start;
		
	}
	
	/**
	 * Gets a key. Calls the four steps - Joker A, Joker B, Triple Cut, Count Cut, then
	 * counts down based on the value of the first card and extracts the next card value 
	 * as key. But if that value is 27 or 28, repeats the whole process (Joker A through Count Cut)
	 * on the latest (current) deck, until a value less than or equal to 26 is found, which is then returned.
	 * 
	 * @return Key between 1 and 26
	 */
	int getKey() {
		// COMPLETE THIS METHOD
		boolean keyFound = false; int key = -1;
		while(keyFound == false){
			jokerA();		//perform step1
			jokerB();		//perform step2
			tripleCut();	//perform step3
			countCut();		//perform step4
			//printList(deckRear);
			int count = deckRear.next.cardValue;
			if(count == 28){
				count = 27;
			}

			CardNode ptr = deckRear.next;
			for(int i = 0; i < (count -1); i++){
				ptr = ptr.next;
			}
			key = ptr.next.cardValue;
			if(key == 27 || key == 28){
				keyFound = false;
			}else break;
		}
		return key;
	}

	/**
	 * Utility method that prints a circular linked list, given its rear pointer
	 * 
	 * @param rear Rear pointer
	 */
	private static void printList(CardNode rear) {
		if (rear == null) { 
			return;
		}
		System.out.print(rear.next.cardValue);
		CardNode ptr = rear.next;
		do {
			ptr = ptr.next;
			System.out.print("," + ptr.cardValue);
		} while (ptr != rear);
		System.out.println("\n");
	}

	/**
	 * Encrypts a message, ignores all characters except upper case letters
	 * 
	 * @param message Message to be encrypted
	 * @return Encrypted message, a sequence of upper case letters only
	 */
	public String encrypt(String message) {	
		// COMPLETE THIS METHOD
	    // THE FOLLOWING LINE HAS BEEN ADDED TO MAKE THE METHOD COMPILE
		String encryptedMessage = "";
		String formattedMessage = message.replaceAll("[^a-zA-Z]","");
		for(int i = 0; i < formattedMessage.length(); i++){
			char ch = formattedMessage.charAt(i);
			int numerical = ch-'A'+1;
			
			int sum = numerical + getKey();
			
			if(sum > 26){
				sum -=26;
			}
			char converted = (char) (sum-1+'A');
			encryptedMessage = encryptedMessage + converted;
		}
	    return encryptedMessage;
	}
	
	/**
	 * Decrypts a message, which consists of upper case letters only
	 * 
	 * @param message Message to be decrypted
	 * @return Decrypted message, a sequence of upper case letters only
	 */
	public String decrypt(String message) {	
		// COMPLETE THIS METHOD
	    // THE FOLLOWING LINE HAS BEEN ADDED TO MAKE THE METHOD COMPILE
		String decryptedMessage = "";
		for(int i = 0; i < message.length(); i++){
			char ch =  message.charAt(i);
			int numerical = ch-'A'+1;
			int key = getKey();
			int sum = numerical - key;
			
			if(numerical <= key){
				sum = (26 + numerical) - key;
			}
			char converted = (char) (sum-1+'A');
			decryptedMessage = decryptedMessage + converted;
		}
	    return decryptedMessage;
	}
}