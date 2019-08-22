
public class NthPrime {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		int n;
		do{
			System.out.println("Enter a value (n > 0) to compute the nth prime number: ");
			n = IO.readInt(); // obtain the value for n from the user; store it in n.
			if(n < 1)
				IO.reportBadInput(); // all integers less than one cannot be stored for n since a sequence can't have a - nth value
		} while (n < 1); // loop until appropriate value is entered
		
		IO.outputIntAnswer(nthPrime(n));
	} // end of main
	
		public static boolean isPrime(int n) { // determines weather a number is prime
		    for(int j = 2; j < n; j++) {
		        if (n % j == 0) // all numbers are inherently divisible by one and itself; we need to test for when isPrime is false
		        {
		            return false;
		        }
		    }
		    return true; // i is only incremented if true is returned
		} // end of isPrime
		
		public static int nthPrime(int n) {
		    int number;
		    int i;
		    for(number = 2, i = 0; i < n; number++) {
		        if (isPrime(number)) //invoke the isPrime() method
		        {
		            i++; //increment i
		        }
		    }
		    // The number has been incremented once after count = n
		    return number - 1; //this value is the nth prime number
		} // end of nthPrime
	
	}// end of class


