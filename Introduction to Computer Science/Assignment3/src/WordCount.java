
public class WordCount {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("Enter a sentence: "); //Prompt the user for a sentence...
		String sentence = IO.readString(); // Store the sentence in a String
		
		System.out.println("Enter a value for the count limit: "); // Prompt the user for a word count threshold.
		int limit = IO.readInt(); // Store the "restriction" for the word-count
		
		System.out.println(WordCount.countWords(sentence, limit)); // call the word count method; pass the sentence and the word-count threshold as parameters
	}
	
	public static int countWords(String original, int minLength){
		if(minLength < 0 || original.isEmpty()){ // error condition; if the threshold is negative or if the String is empty.
			IO.reportBadInput(); // report error
			//return -1; // return a type-sensitive value
		}
		int wordCount = 0;
		original = original.replaceAll("[0-9]", ""); // remove all numbers from the String
		original = original.replaceAll("[!,@,#,$,%,^,&,*,(,),-,_,=,+,{,[,;,',.,/,<,>,?,:,,`,~,|,],}]", ""); // remove all the special characters from the string
		
		String[] words = original.split("\\W+");  // separate the String into into words; denoted by spaces  
		for ( String word : words) { // For-Each loop processes each word in the sentence/String
			 if(word.length() >= minLength) // check if the word exceeds the threshold
				 wordCount++;
		  }
		
		return wordCount; // return the word count for the inputed sentence.
	}
	
}
