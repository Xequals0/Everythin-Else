
public class Poly {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("Enter the first root: ");
		int firstRoot   = IO.readInt(); //obtain the first root from the user
		System.out.println("Enter the second root: ");
		int secondRoot   = IO.readInt(); //obtain the second root from the user
		System.out.println("Enter the third root: ");
		int thirdRoot = IO.readInt(); //obtain the third root from the user
		
		//Assign the roots to the coefficient components: a,b,c for an easier conversion process
		int a = firstRoot;
		int b = secondRoot;
		int c = thirdRoot;
	
		
		// convert polynomial and print output in text form; 
		//if a coefficient is 0 exclude it from the canonical form
		System.out.print("The polynomial is: x^3 ");
		if (a+b+c != 0)
		{
			System.out.print("- "+ (a+b+c)+ "x^2 "); 
		}
		if (a*b + b*c + a*c != 0) 
		{
			System.out.print("+ "+ (a*b + b*c + a*c)+ "x ");
		}
		if (-a*b*c != 0) 
		{
			System.out.print("+ "+(-a*b*c));
		}
			
		
	}
	

}
