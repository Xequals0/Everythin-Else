
public class Scrap {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
	
		System.out.println("Enter text: ");
		String text = IO.readString();
		System.out.println(text);
		
		String compressed = "";
		String reversed = "";
	      char ch=0;
	      int count=1;
	      for (int i = 0; i < text.length(); i++) { 
	         if (ch == text.charAt(i)){
	            count = count + 1;
	         } else {
	            compressed = compressed + ch;
	            if(count != 1){
	               compressed = compressed + count;
	            }
	            ch = text.charAt(i);
	            count = 1;
	         }
	      }
	      compressed = compressed + ch;
	      if(count != 1){
	         compressed = compressed + count;
	      }
	      System.out.println("Compressed: " + compressed); 
	      
	      String[] words = compressed.split("");
	      for(int j = 0; j < words.length; j++){
	    	  if(words[j].equals("0") || words[j].equals("1") || words[j].equals("2") || words[j].equals("3")|| 
	    	     words[j].equals("4") || words[j].equals("5") || words[j].equals("6") || words[j].equals("7")||
	    	     words[j].equals("8") || words[j].equals("9"))
	    	  {
	    		 String temp = words[j];
	    		 words[j] = words[j - 1];
	    		 words[j - 1] = temp; 
	    	  }
	      }		 
	      
	      for(int y = 0; y < words.length; y++){
	    	  reversed += words[y];
	      }
	      
	      System.out.println(reversed);
	     
	      
	      
	      
	
	
	}
		
}


