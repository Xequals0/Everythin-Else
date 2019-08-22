
public class Scores {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		double highest = 10.0; // variable stores highest score to be dropped
		double lowest = 0.0;  // variable stores lowest score to be dropped
		double sum = 0;     // holds the sum of the scores awarded by judges
		int count, judges;
		
		do{	
		System.out.print("Enter the number of judges on the panel: ");
		judges = IO.readInt(); // prompts and stores the number of judges obtained by the user
		if(judges < 3)
		{
			IO.reportBadInput();
			judges = 0;
		}
		} while(judges <= 0);
		
		for(count = 0; count < judges; count++)
		{
			System.out.printf("Enter the score of judge %d: ", count+1);
			double score = IO.readDouble(); // prompts the user to enter the score given by a judge and stores the score
			if(score < 0.0 || score > 10.0)
			{ // removes scores that are not in the acceptable range
				IO.reportBadInput();
				count--;
				continue;
			}
			if(count == 0) //assigns the highest and lowest- dropped scores
			{
				highest = score;
				lowest = score;
			}
			else if(score > highest) // checks to see if new score is greater than the value of the current highest value
				highest = score;
			else if (score < lowest) // checks to see if new score is smaller than the value of the current lowest value
				lowest = score;
			sum += score;
			
		}
		
		sum -= lowest + highest; // drops the highest and lowest scores
		IO.outputDoubleAnswer(sum/(count-2)); // outputs the average score
	}// end of main

}// end of class
