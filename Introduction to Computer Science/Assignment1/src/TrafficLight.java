import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import javax.swing.JComponent;
import javax.swing.JFrame;
import java.awt.Color;
import java.awt.geom.Ellipse2D;

public class TrafficLight extends JComponent
{
	
	 public void paintComponent(Graphics g) {
	 // RecoverGraphics2D 
		 Graphics2D g2 = (Graphics2D) g;

	 // Construct a rectangle and draw it
		 Rectangle box = new Rectangle(50, 100, 100, 220); 
		 g2.draw(box);
		 
		 g2.setColor(Color.red);
		 Ellipse2D.Double one = new Ellipse2D.Double(75, 125, 50, 50);
		 g2.fill(one);
		 
		 g2.setColor(Color.yellow);
		 Ellipse2D.Double two = new Ellipse2D.Double(75, 185, 50, 50);
		 g2.fill(two);
		
		 g2.setColor(Color.green);
		 Ellipse2D.Double three = new Ellipse2D.Double(75, 245, 50, 50);
		 g2.fill(three);
		 
		 
}
	 public static void main(String[] args) {
		// TODO Auto-generated method stub

		JFrame frame = new JFrame();
		frame.setSize(300, 400);
		frame.setTitle("Traffic Light"); 
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		TrafficLight component = new TrafficLight(); 
		frame.add(component);
		frame.setVisible(true);
	 }

}