package structures;

import java.util.*;

/**
 * This class implements an HTML DOM Tree. Each node of the tree is a TagNode, with fields for
 * tag/text, first child and sibling.
 * 		~Anirudh Tunoori
 */
public class Tree {
	
	/**
	 * Root node
	 */
	TagNode root=null;
	
	/**
	 * Scanner used to read input HTML file when building the tree
	 */
	Scanner sc;
	
	/**
	 * Initializes this tree object with scanner for input HTML file
	 * 
	 * @param sc Scanner for input HTML file
	 */
	public Tree(Scanner sc) {
		this.sc = sc;
		root = null;
	}
	
	/**
	 * Builds the DOM tree from input HTML file. The root of the 
	 * tree is stored in the root field.
	 */
	public void build() {
		/** COMPLETE THIS METHOD **/
		root = new TagNode("html",null,null);
		Stack<TagNode> doc = new Stack <TagNode>();
		doc.push(root); sc.nextLine();
		
		while(sc.hasNextLine()){
			String field = sc.nextLine();
			
			//check if the field is an html tag or a text field
			if(field.contains("<") && field.contains(">") && !field.contains("/")){
				field = field.replace("<", ""); field = field.replace(">", "");
				if(doc.peek().firstChild != null){ //there are already multiple tags after html
					TagNode newSibling = doc.peek().firstChild;
					while(newSibling.sibling != null){
					// determining the new tag's location
						newSibling = newSibling.sibling;
					}
					newSibling.sibling =  new TagNode(field, null, null);
					doc.push(newSibling.sibling);
				}
				else
				{//first tag after html
					TagNode add = new TagNode(field, null, null);
					doc.peek().firstChild = add;
					doc.push(add);
				}	
			}else if(field.contains("<") && field.contains(">") && field.contains("/")) doc.pop(); //The DOM tree does not hold the end tags
		
			else{//there are already multiple text fields in this height
				if(doc.peek().firstChild != null){
					TagNode newText = doc.peek().firstChild;
					while(newText.sibling != null){
					// determine the new text field's location
						newText = newText.sibling;
					} newText.sibling =  new TagNode(field, null, null);
				}else
				{//the field is not a tag, it is a text field
					doc.peek().firstChild = new TagNode(field, null, null);
				}
			}	
		}//end loop
	}//end build
	
	
	/**
	 * Replaces all occurrences of an old tag in the DOM tree with a new tag
	 * 
	 * @param oldTag Old tag
	 * @param newTag Replacement tag
	 */
	public void replaceTag(String oldTag, String newTag) {
		/** COMPLETE THIS METHOD **/
		if(root == null || oldTag == null || newTag == null) return;	
		
		if((oldTag.equals("em") || oldTag.equals("b") || oldTag.equals("p")) && 
		   (newTag.equals("ol") || newTag.equals("ul"))) return;
		
		if((oldTag.equals("ol") || oldTag.equals("ul")) && 
		   (newTag.equals("em") || newTag.equals("b") || newTag.equals("p"))) return;
		
		replace(oldTag, newTag, root);
	}
	//recursive helper method for replaceTag
	private static void replace(String oldTag, String newTag, TagNode currentNode){
		if(currentNode == null) return; //base case
		if(currentNode.tag.equals(oldTag))
			currentNode.tag = newTag;	//replace the tag
		//recursive case: traverse the tree
		replace(oldTag, newTag, currentNode.firstChild);	
		replace(oldTag, newTag, currentNode.sibling);
	}
	
	/**
	 * Boldfaces every column of the given row of the table in the DOM tree. The boldface (b)
	 * tag appears directly under the td tag of every column of this row.
	 * 
	 * @param row Row to bold, first row is numbered 1 (not 0).
	 */
	public void boldRow(int row) {
		/** COMPLETE THIS METHOD **/
		if(row <= 0) return;
		embolden(row, root);
	}
	//recursive helper method for boldRow
	private static void embolden(int row, TagNode root){
		if(root == null) return;	//base case
		if(root.tag.equals("table")){
			TagNode tr = root.firstChild;
			for(int i=1; i < row; i++){
				tr = tr.sibling;	//Acquire the correct tr
			}
			TagNode td = tr.firstChild;
			td.firstChild = new TagNode("b", td.firstChild, null);
			while(td.sibling != null){
				td = td.sibling;
				td.firstChild = new TagNode("b", td.firstChild, null);
			}
		}	//recursive case: traverse the tree until a table is found
			embolden(row, root.firstChild);
			embolden(row, root.sibling);
	}
	/**
	 * Remove all occurrences of a tag from the DOM tree. If the tag is p, em, or b, all occurrences of the tag
	 * are removed. If the tag is ol or ul, then All occurrences of such a tag are removed from the tree, and, 
	 * in addition, all the li tags immediately under the removed tag are converted to p tags. 
	 * 
	 * @param tag Tag to be removed, can be p, em, b, ol, or ul
	 */
	public void removeTag(String tag) {
		/** COMPLETE THIS METHOD **/
		if(!(tag.equals("p") || tag.equals("em") || tag.equals("b") || tag.equals("ol") || tag.equals("ul")))	
			return;
		if(root == null) return;
		
		while(searchForTag(tag, root)){//searches to see if the tag can still be found within the tree
			recursiveRemoval(tag, root.firstChild, root);
		}
	}
	private static boolean searchForTag(String tag, TagNode ptr){
		if(ptr == null) return false;	//base case
		if(ptr.tag.equals(tag)) return true;	//determine if the loop should continue
		//recursive case occurs if more of the tree can be searched 
		//and the current node does not have the target tag
		return searchForTag(tag, ptr.firstChild) || searchForTag(tag, ptr.sibling);	//returns true if a tag can still be found; false if not
	}
	
	private static void recursiveRemoval(String tag, TagNode ptr, TagNode prev){
		if(ptr == null || prev == null) return; //base case of recursive removal; occurs once the desired tag has been removed
		
		if(ptr.tag.equals(tag)){
			if(tag.equals("ol") || tag.equals("ul")) litop(ptr.firstChild);	//convert all li's top's
			if(prev.firstChild == ptr){
				prev.firstChild = ptr.firstChild;
				while(ptr.firstChild.sibling != null){//adds a new sibling to the current last sibling at this "height"
					ptr.firstChild =ptr.firstChild.sibling;
				}ptr.firstChild.sibling = ptr.sibling; //add the new last sibling
			}else if(prev.sibling == ptr){
				while(ptr.firstChild.sibling != null){//adds a new sibling to the current last sibling at this "height"
					ptr.firstChild = ptr.firstChild.sibling;
				}ptr.firstChild.sibling = ptr.sibling; //add the new last sibling
				prev.sibling = ptr.firstChild;	
			}return;
		}//tag has not been found...
		prev = ptr;	//shift prev, and then continue traversing through the tree using the following recursive calls
		recursiveRemoval(tag, ptr.firstChild, prev);
		recursiveRemoval(tag, ptr.sibling, prev);
	}
	private static void litop(TagNode temp){
		if(temp == null) return; //base case
		if(temp.tag.equals("li")) temp.tag = "p";	//replaces the li tag's with p
		litop(temp.sibling);	//recursive call
	}
	
	/**
	 * Adds a tag around all occurrences of a word in the DOM tree.
	 * 
	 * @param word Word around which tag is to be added
	 * @param tag Tag to be added
	 */
	public void addTag(String word, String tag) {
		/** COMPLETE THIS METHOD **/
		if((!tag.equals("em") && !tag.equals("b")) || root == null) return;
		recursiveAdd(word, tag, root.firstChild);
	}
	private static void recursiveAdd(String word, String tag, TagNode current){
		if(current == null) return;	//base case
		else if (current.tag.toLowerCase().contains(word.toLowerCase())) {			
			if (current.tag.equalsIgnoreCase(word)) {//the tag consists of exactly this word and this node has no siblings
				String oldWord = current.tag;
				current.tag = tag;	//add the tag
				current.firstChild = new TagNode (oldWord, current.firstChild, null);
			}else{
				 String text = current.tag; 
				int textStart = text.toLowerCase().indexOf(word.toLowerCase());
							
				String oldWord = text.substring(textStart, textStart + word.length());	//contains the original word
				TagNode nextWord = current.sibling;
				String afterWord = text.substring(textStart + word.length());	// contains everything after the target word
				String punctuation = "";	//will hold any punctuation found on the targeted word
				String beforeWord = text.substring(0, textStart);	//contains everything before the target word

				if((afterWord.length() > 1)){	//if after word is greater than one the characters should be checked
					char p = afterWord.charAt(0); char pPlus = afterWord.charAt(1);//holds the first punctuation and the second , respectively
				if((p == '.' || p == ',' || p == '?' || p == '!' ||p == ':' || p == ';')
				&& (pPlus != '.' || pPlus != ',' || pPlus != '?' || pPlus != '!' || pPlus != ':' || pPlus != ';')){
					punctuation = "" + p;	//adds the valid punctuation
					afterWord = afterWord.substring(1);	//removes the punctuation from the afterWord
				}	p = afterWord.charAt(0);	//store punctuation once again
				}
			if (afterWord.length() == 0 || (afterWord.length() >= 1 && 
			   (afterWord.charAt(0) == '.' || afterWord.charAt(0) == ',' || afterWord.charAt(0) == '?' || afterWord.charAt(0) == '!' ||afterWord.charAt(0) == ':' || afterWord.charAt(0) == ';' || afterWord.charAt(0) == ' '))){
				
				if (afterWord.equals("!") || afterWord.equals(",") || 
					afterWord.equals(".") || afterWord.equals("?") || afterWord.equals(";") || afterWord.equals(":")) {
					oldWord = oldWord + afterWord;
					oldWord = "";	//empty oldWord
				}	current.tag = beforeWord;	//restore the original word; matches with the target
				TagNode temp = new TagNode(oldWord + punctuation, null, null);
				current.sibling = new TagNode(tag, temp, null);
				
				if (afterWord.length() > 0) {
					if (nextWord != null) //if nextWord exists, make it current.sibling.sibling's sibling
						current.sibling.sibling = new TagNode(afterWord, null, nextWord);
					else
						current.sibling.sibling = new TagNode(afterWord, null, null);
				} else if (nextWord != null) current.sibling.sibling = nextWord;	//advance current.sibling.sibling
			}
			}	recursiveAdd(word, tag, current.sibling.sibling);	//evaluate with the next sibling
		}else //continue the tree traversal
		{ recursiveAdd(word, tag, current.firstChild); 
		  recursiveAdd(word, tag, current.sibling);
		}
	}
	/**
	 * Gets the HTML represented by this DOM tree. The returned string includes
	 * new lines, so that when it is printed, it will be identical to the
	 * input file from which the DOM tree was built.
	 * 
	 * @return HTML string, including new lines. 
	 */
	public String getHTML() {
		StringBuilder sb = new StringBuilder();
		getHTML(root, sb);
		return sb.toString();
	}
	
	private void getHTML(TagNode root, StringBuilder sb) {
		for (TagNode ptr=root; ptr != null;ptr=ptr.sibling) {
			if (ptr.firstChild == null) {
				sb.append(ptr.tag);
				sb.append("\n");
			} else {
				sb.append("<");
				sb.append(ptr.tag);
				sb.append(">\n");
				getHTML(ptr.firstChild, sb);
				sb.append("</");
				sb.append(ptr.tag);
				sb.append(">\n");	
			}
		}
	}
	
}
