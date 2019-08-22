
public class scrap {
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println(decompress("q9w5e2rt5y4qw2Er3T"));
	}
	public static String decompress(String compressedText){
	    char c = ' ';
    String temp;
    if(compressedText.length() == 1){
        return compressedText;       
    }
    if(compressedText.length() != 1){
        c = compressedText.charAt(1);
    }else if(compressedText.charAt(0) == '0' && compressedText.length() != 2){
        return decompress(compressedText.substring(2));
    }else if(compressedText.charAt(0) == '0' && compressedText.length() == 2){
        compressedText = "\0";
        return compressedText;
    }else if(Character.isDigit(compressedText.charAt(0))){
        int i = Integer.parseInt(compressedText.substring(0,1));
        i = i-1;
        temp = Integer.toString(i);
        return c + decompress(temp.concat(compressedText.substring(1)));
    }
    return compressedText.charAt(0)+decompress(compressedText.substring(1));
	}
}