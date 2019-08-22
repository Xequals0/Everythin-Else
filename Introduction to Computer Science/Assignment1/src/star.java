import java.util.Scanner;

//1, 2, 3, 4, 6, 9, 11, 14, 19, 20
public class star {
	
	public static void main(String[] args) {
		
		Scanner in = new Scanner(System.in);
		int a = 400;
		int b = 499;
		for(int i = 5; i >= 1; i--){
			if(i!=1){
			System.out.println("Century " + i + " BC " + a + "" + b);
			}
			else
				System.out.println("Century " + i + " BC " + (b+100));
			a-= 100;
			b-= 100;
			
		}	
		a = 0;
	 b = 99;
		for(int i = 1; i <= 5; i++){
		if(i!=1){
			System.out.println("Century " + i + " AD " + a + "" + b);
			}
			else
				System.out.println("Century " + i + " AD " + (199));
			a+= 100;
			b+= 100;
		}
		
	} // end of main
}//end of class
