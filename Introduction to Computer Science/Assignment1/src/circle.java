import java.util.Scanner;

import javax.swing.JComponent;
import javax.swing.JFrame;

import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.geom.Ellipse2D;

public class circle  extends JComponent{
	
	private int r1;
	private int r2;
	
	public circle(int r1, int r2){
		this.r1 = r1;
		this.r2 = r2;
	}
	public boolean intersects(){
		int onex = 100 - this.r1;
		int oney = 200 - this.r1;
		int twox = 200 - this.r2;
		int twoy = 100 - this.r2;
		double distance = Math.sqrt((onex-twox)*(onex-twox) + (oney-twoy)*(oney-twoy));
		if(distance > (this.r1 + this.r2)){
			return false;
		}
		return true;
	}

	public void paintComponent(Graphics g) {
		 // RecoverGraphics2D 
			 Graphics2D g2 = (Graphics2D) g;

			 if (intersects()){
				 g2.setColor(Color.GREEN);
			 }else g2.setColor(Color.RED);
			 Ellipse2D.Double one = new Ellipse2D.Double(100 - this.r1, 200 - this.r1, 2*this.r1, 2*this.r1);
			 g2.fill(one);
			 
			 Ellipse2D.Double two = new Ellipse2D.Double(200 - this.r2, 100 - this.r2, 2*this.r2, 2*this.r2);
			 g2.fill(two);
			 
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Scanner in = new Scanner(System.in);
		
		System.out.println("Please enter the radius (integer only) of the first circle: ");
		int r1 = in.nextInt();
		if(r1<0)return;
		System.out.println("Please enter the radius (integer only) of the second circle: ");
		int r2 = in.nextInt();
		if(r2<0)return;
		
		circle component = new circle(r1, r2);
		JFrame frame = new JFrame();
		frame.setSize(400, 400);
		frame.setTitle("Circle"); 
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		frame.add(component);
		frame.setVisible(true);
	}
}
