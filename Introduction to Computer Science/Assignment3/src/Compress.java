
public class Compress {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("Enter text: "); //Prompt the user for text to RLE
		String text = IO.readString(); // store the entered text as a String
		
		System.out.println(compress(text)); // Call the compress method; pass the text as the parameter.
	}
	
	public static String compress (String original){
		String compressed = ""; // holds a compressed version of the text
		String reversed = ""; // holds the reversed (desired/correct) version of the compressed text
	      char in=0; // holds the letters of the text
	      int count=1; // counter
	      
	      for (int i = 0; i < original.length(); i++) { // runs through every character of the original String.
	         if (in == original.charAt(i))
	         {
	            count++; // increments count when a character is consecutively repeated
	         }
	         else 
	         {
	            compressed = compressed + in; //adds a new character/initial to the already compressed string
	            if(count != 1) // executed after a character stops repeating consecutively
	            {
	               compressed = compressed + count;
	            }
	            in = original.charAt(i); //updates the next initial/character
	            count = 1; // sets count back to 1 for the next initial/character
	         }
	      } // break out of loop
	      compressed = compressed + in; // adds a character to the compressed string
	      if(count != 1)
	      {
	         compressed = compressed + count; // adds count after each repeated character
	      }
	      
	      String[] data = compressed.split(""); // split up the compressed String into a String[] so that each element holds a character.
	      for(int j = 0; j < data.length; j++){ // reverse the order of the digits/count and the repeated character.
	    	  if(data[j].equals("0") || data[j].equals("1") || data[j].equals("2") || data[j].equals("3")|| 
	    		 data[j].equals("4") || data[j].equals("5") || data[j].equals("6") || data[j].equals("7")||
	    		 data[j].equals("8") || data[j].equals("9"))
	    	  {
	    		 String temp = data[j]; // Reverses the order.
	    		 data[j] = data[j - 1];
	    		 data[j - 1] = temp; 
	    	  }
	      }		 
	      
	      for(int y = 0; y < data.length; y++){
	    	  reversed += data[y]; // returns the String[] back into one single String.
	      }
	      
		return reversed; // returns the compressed text in the correct RLE format.
	}
}
