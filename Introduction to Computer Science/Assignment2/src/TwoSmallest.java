
public class TwoSmallest {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		double min1 = 0; // smallest
		double min2 = 0; // second smallest
		double terminator, number; // terminator value and number

		
			System.out.print("Enter termination value (any real number): ");
			terminator = IO.readDouble(); // obtain the terminal value
			while(true){
				System.out.print("Enter a real number (must enter at least two before terminator): ");
				number = IO.readDouble();
				if(number == terminator)
					IO.reportBadInput();
				else 
					break; // break out of loop/error condition if number != terminal
			}
		min1 = number; // give
		min2 = number; // min 1 and 2 initial values
		int count = 0;
		
		while(number != terminator){
			System.out.print("Enter the next (real) number, or terminator: ");
			number = IO.readDouble();
			if (number == terminator && count == 0) // this branch is if only one number is entered between terminal values
			{
				while(true){
					System.out.print("Enter the next real number (must enter at least two before terminator): ");
					number = IO.readDouble();
					if (number == terminator) // report error condition
						IO.reportBadInput();
					else break;
				}// leave error condition this, "box" is entered at most once
				count++;
			}
			if (number == terminator && count > 1)
				break; // if terminal is inputed and there is more than one non terminator number in the sequence
		    if (number < min1)
			{
				min2 = min1;  // swap values of min 1 and 2
		    	min1 = number;
			}
		    else if (number < min2)
		    	min2 = number;
		    //else if (min1 == min2)// special case error condition for only two numbers
		  //  	min2 = number; // usually unneeded
			count++;
			}
		
			IO.outputDoubleAnswer(min1); // output both the smallest
			IO.outputDoubleAnswer(min2); // and the second smallest
	}

}


