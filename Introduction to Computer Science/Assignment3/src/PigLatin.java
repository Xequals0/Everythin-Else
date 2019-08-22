
public class PigLatin {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("Enter a word to translate (to Pig Latin): "); //Prompt the user for a word to translate
		String word = IO.readString(); // Store the word to translate
		
		System.out.println(PigLatin.translate(word)); // call the translate method; pass the word as the parameter
	}
	
	public static String translate (String original){
		boolean isVowel = false; // boolean value used to determine if the first letter is a vowel or a consonant
		
		if(original.substring(0,1).equalsIgnoreCase("a") || // check for all the possible vowel cases
		   original.substring(0,1).equalsIgnoreCase("e") || 
		   original.substring(0,1).equalsIgnoreCase("i") ||
		   original.substring(0,1).equalsIgnoreCase("o") ||
		   original.substring(0,1).equalsIgnoreCase("u"))
		{ 
			isVowel = true; // set boolean to true
		}
		
		if(isVowel){ // apply rule for vowel beginning
			return original + "way";
		}
		else // apply rule for vowel beginning
			return original.substring(1,original.length()) 
				 + original.substring(0,1) + "ay";	
	}
	
}
