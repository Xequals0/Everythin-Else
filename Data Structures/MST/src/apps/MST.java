package apps;

import structures.*;

import java.io.IOException;
import java.util.ArrayList;
//import java.util.Iterator;

public class MST {
	
	/**
	 * Initializes the algorithm by building single-vertex partial trees
	 * 
	 * @param graph Graph for which the MST is to be found
	 * @return The initial partial tree list
	 * ~Anirudh Tunoori
	 */
	public static PartialTreeList initialize(Graph graph) {
		/* COMPLETE THIS METHOD */
		int size = graph.vertices.length;
		PartialTreeList List = new PartialTreeList();	//Step 1: Create an empty list L of partial trees
		//Step 2:
		for(int i = 0; i < size; i++){
			//Create a partial tree T containing only v; Mark v as belonging to T
			PartialTree Tree = new PartialTree(graph.vertices[i]);
			//Obtain the neighbors of this vertex
			Vertex.Neighbor currentNeighbors = graph.vertices[i].neighbors;
			//Create a priority queue (heap) P and associate it with T
			MinHeap<PartialTree.Arc> arcWeight = Tree.getArcs();
			
			//Insert all of the arcs (edges) connected to v into P. The lower the weight on an arc, the higher its priority.
			while(currentNeighbors != null){
				PartialTree.Arc edge = new PartialTree.Arc(graph.vertices[i], currentNeighbors.vertex, currentNeighbors.weight);
				arcWeight.insert(edge);
				if(currentNeighbors.next == null) break;
				currentNeighbors = currentNeighbors.next;
			}//end while
			//Add the partial tree T to the list L:
			System.out.println(Tree.toString());
			List.append(Tree);	
		}//end for
		return List;
	}//end of initialize
	
	//main method for testing purposes...
	public static void main(String[]args) throws IOException{
		String s = "graph1.txt";
		Graph g = new Graph(s);
		
		PartialTreeList pt = initialize(g);
		pt.remove();
		//removeTreeContaining();
		//ArrayList<PartialTree.Arc> MSArcsList =  execute(pt);
		System.out.println(pt.toString());
		
	}
	//helper method
	private static boolean containsV(Vertex vertex, PartialTree tree){
		MinHeap<PartialTree.Arc> arc = tree.getArcs();
		//Iterator<PartialTree.Arc> i = arc.iterator();
		for(PartialTree.Arc A : arc){
			Vertex one = A.v1;
			if(vertex.equals(one))	return true;
		}
		/*
		while(i.hasNext()){
				Vertex one = i.next().v1;
				if(vertex.equals(one))	return true;
			}*/
		return false;
	}//end of contains
	/**
	 * Executes the algorithm on a graph, starting with the initial partial tree list
	 * 
	 * @param ptlist Initial partial tree list
	 * @return Array list of all arcs that are in the MST - sequence of arcs is irrelevant
	 */
	public static ArrayList<PartialTree.Arc> execute(PartialTreeList ptlist) {
		/* COMPLETE THIS METHOD */
		ArrayList<PartialTree.Arc> MSArcsList = new ArrayList<PartialTree.Arc>();
		int i = ptlist.size();
		while(!(i<=1)){
			//Step 3: Remove the first partial tree PTX from L. Let PQX be PTX's priority queue
			PartialTree t = ptlist.remove();
			MinHeap<PartialTree.Arc> arcWeight = t.getArcs();
			if(arcWeight == null) break;
			//Step 4: Remove the highest-priority arc from PQX. Say this arc is α. Let v1 and v2 be the two vertices connected by α, where v1 belongs to PTX
			PartialTree.Arc maxEdge = arcWeight.deleteMin();
			Vertex two = maxEdge.v2;
			//Step 5: If v2 also belongs to PTX, go back to Step 4 and pick the next highest priority arc, otherwise continue to the next step
			while(containsV(two,t)){
				maxEdge = arcWeight.deleteMin();
				two = maxEdge.v2;
			}
				//Step 6: Report α: a component of the minimum spanning tree
			MSArcsList.add(maxEdge);
			//Step 7: Find the partial tree PTY to which v2 belongs. Remove PTY from the partial tree list L. Let PQY be PTY's priority queue.
			PartialTree t2 = ptlist.removeTreeContaining(two);
			//Step 8:Combine PTX and PTY. This includes merging the priority queues PQX and PQY into a single priority queue. Append the resulting tree to the end of L.
			//t2.getRoot().parent = t.getRoot();
			t.merge(t2);
			ptlist.append(t);
			i--;
		}
		return MSArcsList;
	}//end of execute
}
