public class StringRec
{
	// DO NOT DECLARE ANY VARIABLES HERE
	// (you may declare local variables inside methods)
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println(decompress("5g 6h"));
	}
	public static String decompress(String compressedText)
	{
		String convertCounter;	//holds the counter as a string
		int count;				//holds any digits as an int
		char currentLetter = ' '; // The current letter being decompressed
		
		if(compressedText.equals("") || compressedText.equals(null)){
			return "";	// if the text is empty or null return nothing: empty String
		}
		if(compressedText.length() != 1){
			currentLetter = compressedText.charAt(1);	//hold the second character in the string
		}

		if(compressedText.length() == 1){
			return compressedText;	//if the text is only one character, simply return that character.
		}
		else if(compressedText.charAt(0) == '0' && compressedText.length() == 2){
	        return "";	//example case: 0a should return nothing 
	    }
		else if(compressedText.charAt(0) == '0' && compressedText.length() != 2){
	        return decompress(compressedText.substring(2));	// if the number before a letter is a 0, that letter has been successfully compressed.
	    }													//decompress the rest of the String.
	    else if(Character.isDigit(compressedText.charAt(0))){
			count = Integer.parseInt(compressedText.substring(0,1));	// convert a number "character" to an integer
			if(count != 0){	// check to see if the count is not 0
				count--;	// decrement count for this recurse
				convertCounter = Integer.toString(count);	//convert count back to a String after decerementing.
				return currentLetter + decompress(convertCounter + compressedText.substring(1));	//return->recurse the letter, the updated count plus the rest of the String
			}
		}
		return  compressedText.charAt(0) + decompress(compressedText.substring(1));	//proceed to the next character...
	}
}