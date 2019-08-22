//import java.lang.Math;
public class Intersect {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		/*
		 * a degree-2 (quadratic) polynomial i.e.  
			y = dx^2 + fx + g
			and a degree-1 (linear) equation i.e.
			y = mx + b
		 */
		System.out.println("Enter the constant d: ");
		double d   = IO.readDouble(); //obtain the constant d from the user
		
		System.out.println("Enter the constant f: ");
		double f   = IO.readDouble(); //obtain the constant f from the user
		
		System.out.println("Enter the constant g: ");
		double g   = IO.readDouble(); //obtain the constant g from the user
		
		System.out.println("Enter the constant m: ");
		double m   = IO.readDouble(); //obtain the constant m from the user
		
		System.out.println("Enter the constant b: ");
		double b   = IO.readDouble(); //obtain the constant b from the user
		
		//Finding the intersection points; Use the quadratic formula to find the ordered pair
		// ax^2 + bx + c
		
		//First set the user-obtained constants in terms of quadratic coefficients: A,B,C
		double A = d;
		double B = f-m;
		double C = g-b;
		
		//Use a Mathematical process to find the coordinates of the two ordered pairs
		//i.e: Quadratic Formula
		double x1 = (-B + Math.sqrt(B*B - 4*A*C))/(2*A);
		double y1 = (m*x1) + b;
		
		double x2 = (-B - Math.sqrt(B*B - 4*A*C))/(2*A);
		double y2 = (m*x2) + b;
		
		//Output the intersection point(s) as a coordinate pair
		//Given the input conditions there are 6 possible intersection cases
		if(d == 0 && f == m && g ==b) // two coinciding lines
		{
			System.out.println("There are infinitely many intersections.");
		}
		else if (d == 0 && f != m) //two lines that intersect at one point
		{
			x1 = (b - g)/(f - m);
			y1 = m*x1 + b;
			System.out.println("The intersection is: (" +x1+","+y1+")");// output the intersection point only once
		}
		else if (Double.isNaN(x1) || Double.isNaN(x2)) // either two parallel lines or a quadratic and linear function that never intersect
		{
			System.out.println("none");
		}

		else if(x1==x2 && y1==y2)
		{
			System.out.println("The intersection is: (" +x1+","+y1+")"); //a line that is tangential to a parabola at exactly one point
		}
		else
		{
			System.out.println("The intersection(s) is/are: "); // all other cases: a quadratic and linear function that intersect at two points
			System.out.println("(" +x1 +","+y1+")");
			System.out.println("(" +x2 +","+y2+")");
		}	
	
	}

}
