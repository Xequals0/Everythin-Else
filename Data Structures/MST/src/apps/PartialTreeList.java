package apps;

import java.util.Iterator;
import java.util.NoSuchElementException;

import structures.MinHeap;
import structures.Vertex;


public class PartialTreeList implements Iterable<PartialTree> {
    
	/**
	 * Inner class - to build the partial tree circular linked list 
	 * ~Anirudh Tunoori
	 */
	public static class Node {
		/**
		 * Partial tree
		 */
		public PartialTree tree;
		
		/**
		 * Next node in linked list
		 */
		public Node next;
		
		/**
		 * Initializes this node by setting the tree part to the given tree,
		 * and setting next part to null
		 * 
		 * @param tree Partial tree
		 */
		public Node(PartialTree tree) {
			this.tree = tree;
			next = null;
		}
	}

	/**
	 * Pointer to last node of the circular linked list
	 */
	private Node rear;
	
	/**
	 * Number of nodes in the CLL
	 */
	private int size;
	
	/**
	 * Initializes this list to empty
	 */
    public PartialTreeList() {
    	rear = null;
    	size = 0;
    }

    /**
     * Adds a new tree to the end of the list
     * 
     * @param tree Tree to be added to the end of the list
     */
    public void append(PartialTree tree) {
    	Node ptr = new Node(tree);
    	if (rear == null) {
    		ptr.next = ptr;
    	} else {
    		ptr.next = rear.next;
    		rear.next = ptr;
    	}
    	rear = ptr;
    	size++;
    }
   
    public static void printNodes(PartialTreeList a)
    {		
    	if(a.rear == null)	System.out.print("[]");			

    	else
    	{			
    		Node cur = a.rear.next;
    		while(cur != a.rear)
    		{
    			System.out.println("->[" + cur.tree + "]");
    			cur = cur.next;
    		}
    		System.out.println("->[" + cur.tree.toString() + "]->");			
    	}			System.out.println();
    }
    /**
     * Removes the tree that is at the front of the list.
     * 
     * @return The tree that is removed from the front
     * @throws NoSuchElementException If the list is empty
     */
    public PartialTree remove() throws NoSuchElementException {
    	/* COMPLETE THIS METHOD */
    	if(rear == null) throw new NoSuchElementException("CLL is empty");
    	// There are two types of removal:
    	if(rear == rear.next){//single node in the CLL
    		PartialTree T = rear.tree;
    		rear = null; size = 0;
    		return T;
    	}else if(rear.next != null){//multiple nodes left in the CLL
    		PartialTree T = rear.next.tree;
    		rear.next = rear.next.next;	size--;
    		return T;
    	}
    	return null;
    }
    //helper method
    private static boolean containsV(Vertex vertex, PartialTree tree){
		MinHeap<PartialTree.Arc> arc = tree.getArcs();
		Iterator<PartialTree.Arc> i = arc.iterator();
			while(i.hasNext()){
				Vertex one = i.next().v1;
				if(vertex.equals(one))	return true;
			}
		return false;
	}
    /**
     * Removes the tree in this list that contains a given vertex.
     * 
     * @param vertex Vertex whose tree is to be removed
     * @return The tree that is removed
     * @throws NoSuchElementException If there is no matching tree
     */
   
    public PartialTree removeTreeContaining(Vertex vertex) throws NoSuchElementException {
    	/* COMPLETE THIS METHOD */
    	PartialTree PT = rear.tree;
    	Node ptr = rear.next;
    	Node prev = rear;
    	if(rear == null) throw new NoSuchElementException("CLL is empty");
    	if(rear == rear.next){
    		if(containsV(vertex,rear.tree)){	//check if the vertex matches the root of the partial tree
    			PT = rear.tree;
    			rear = null; size = 0;
    			return PT;
    		}
    	}do{
    		if(containsV(vertex,ptr.tree)){
    			PT = ptr.tree;
    			prev.next = ptr.next;
    			if(ptr == rear)	rear = prev;
    			return PT;
    		}//advance pointers:
    		prev = ptr;	ptr = ptr.next;
    	}while(prev != rear);//end while
    	if(ptr == rear) throw new NoSuchElementException("there is no matching tree");
    		size--;
    		return PT;
     }
    
    /**
     * Gives the number of trees in this list
     * 
     * @return Number of trees
     */
    public int size() {
    	return size;
    }
    
    /**
     * Returns an Iterator that can be used to step through the trees in this list.
     * The iterator does NOT support remove.
     * 
     * @return Iterator for this list
     */
    public Iterator<PartialTree> iterator() {
    	return new PartialTreeListIterator(this);
    }
    
    private class PartialTreeListIterator implements Iterator<PartialTree> {
    	
    	private PartialTreeList.Node ptr;
    	private int rest;
    	
    	public PartialTreeListIterator(PartialTreeList target) {
    		rest = target.size;
    		ptr = rest > 0 ? target.rear.next : null;
    	}
    	
    	public PartialTree next() 
    	throws NoSuchElementException {
    		if (rest <= 0) {
    			throw new NoSuchElementException();
    		}
    		PartialTree ret = ptr.tree;
    		ptr = ptr.next;
    		rest--;
    		return ret;
    	}
    	
    	public boolean hasNext() {
    		return rest != 0;
    	}
    	
    	public void remove() 
    	throws UnsupportedOperationException {
    		throw new UnsupportedOperationException();
    	}
    	
    }
}


