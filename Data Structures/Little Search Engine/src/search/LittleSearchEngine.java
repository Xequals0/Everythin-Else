package search;

import java.io.*;
import java.util.*;

/**
 * This class encapsulates an occurrence of a keyword in a document. It stores the
 * document name, and the frequency of occurrence in that document. Occurrences are
 * associated with keywords in an index hash table.
 * 
 * @author Sesh Venugopal
 * ~Anirudh Tunoori
 */
class Occurrence {
	/**
	 * Document in which a keyword occurs.
	 */
	String document;
	
	/**
	 * The frequency (number of times) the keyword occurs in the above document.
	 */
	int frequency;
	
	/**
	 * Initializes this occurrence with the given document,frequency pair.
	 * 
	 * @param doc Document name
	 * @param freq Frequency
	 */
	public Occurrence(String doc, int freq) {
		document = doc;
		frequency = freq;
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		return "(" + document + "," + frequency + ")";
	}
}

/**
 * This class builds an index of keywords. Each keyword maps to a set of documents in
 * which it occurs, with frequency of occurrence in each document. Once the index is built,
 * the documents can searched on for keywords.
 *
 */
public class LittleSearchEngine {
	
	/**
	 * This is a hash table of all keywords. The key is the actual keyword, and the associated value is
	 * an array list of all occurrences of the keyword in documents. The array list is maintained in descending
	 * order of occurrence frequencies.
	 */
	HashMap<String,ArrayList<Occurrence>> keywordsIndex;
	
	/**
	 * The hash table of all noise words - mapping is from word to itself.
	 */
	HashMap<String,String> noiseWords;
	
	/**
	 * Creates the keyWordsIndex and noiseWords hash tables.
	 */
	public LittleSearchEngine() {
		keywordsIndex = new HashMap<String,ArrayList<Occurrence>>(1000,2.0f);
		noiseWords = new HashMap<String,String>(100,2.0f);
	}
	
	/**
	 * This method indexes all keywords found in all the input documents. When this
	 * method is done, the keywordsIndex hash table will be filled with all keywords,
	 * each of which is associated with an array list of Occurrence objects, arranged
	 * in decreasing frequencies of occurrence.
	 * 
	 * @param docsFile Name of file that has a list of all the document file names, one name per line
	 * @param noiseWordsFile Name of file that has a list of noise words, one noise word per line
	 * @throws FileNotFoundException If there is a problem locating any of the input files on disk
	 */
	public void makeIndex(String docsFile, String noiseWordsFile) 
	throws FileNotFoundException {
		// load noise words to hash table
		Scanner sc = new Scanner(new File(noiseWordsFile));
		while (sc.hasNext()) {
			String word = sc.next();
			noiseWords.put(word,word);
		}
		
		// index all keywords
		sc = new Scanner(new File(docsFile));
		while (sc.hasNext()) {
			String docFile = sc.next();
			HashMap<String,Occurrence> kws = loadKeyWords(docFile);
			mergeKeyWords(kws);
		}
		
	}

	/**
	 * Scans a document, and loads all keywords found into a hash table of keyword occurrences
	 * in the document. Uses the getKeyWord method to separate keywords from other words.
	 * 
	 * @param docFile Name of the document file to be scanned and loaded
	 * @return Hash table of keywords in the given document, each associated with an Occurrence object
	 * @throws FileNotFoundException If the document file is not found on disk
	 */
	public HashMap<String,Occurrence> loadKeyWords(String docFile) 
	throws FileNotFoundException {
		// COMPLETE THIS METHOD
		// THE FOLLOWING LINE HAS BEEN ADDED TO MAKE THE METHOD COMPILE
		if(docFile == null){
			throw new FileNotFoundException("File not found..");
		}
		Scanner sc = new Scanner(new File(docFile));
		HashMap<String,Occurrence> load = new HashMap<String, Occurrence>();	
		while(sc.hasNext()){
			String word = getKeyWord(sc.next());
			if(word != null){
				if(load.containsKey(word)){	//Recurrence
					load.get(word).frequency++;
				}else {load.put(word, new Occurrence(docFile,1));	//new occurrence
			}
		}
		}return load;
	}
	
	/**
	 * Merges the keywords for a single document into the master keywordsIndex
	 * hash table. For each keyword, its Occurrence in the current document
	 * must be inserted in the correct place (according to descending order of
	 * frequency) in the same keyword's Occurrence list in the master hash table. 
	 * This is done by calling the insertLastOccurrence method.
	 * 
	 * @param kws Keywords hash table for a document
	 */
	public void mergeKeyWords(HashMap<String,Occurrence> kws) {
		for (String key : kws.keySet()){
			ArrayList<Occurrence> rejected = new ArrayList<Occurrence>(); 
			Occurrence occurrences = kws.get(key);
			if(keywordsIndex.containsKey(key)){
				keywordsIndex.get(key).add(occurrences);
				insertLastOccurrence(keywordsIndex.get(key));
				keywordsIndex.put(key,keywordsIndex.get(key));
			}else{
				rejected.add(occurrences);
				keywordsIndex.put(key,rejected);
			}
		}
	}

	
	/**
	 * Given a word, returns it as a keyword if it passes the keyword test,
	 * otherwise returns null. A keyword is any word that, after being stripped of any
	 * TRAILING punctuation, consists only of alphabetic letters, and is not
	 * a noise word. All words are treated in a case-INsensitive manner.
	 * 
	 * Punctuation characters are the following: '.', ',', '?', ':', ';' and '!'
	 * 
	 * @param word Candidate word
	 * @return Keyword (word without trailing punctuation, LOWER CASE)
	 */
	public String getKeyWord(String word) {	
		// COMPLETE THIS METHOD
		// THE FOLLOWING LINE HAS BEEN ADDED TO MAKE THE METHOD COMPILE
		if(word.length() == 1) return null;
		for(int i = 0; i < word.length(); i++){
			char c = word.charAt(i);
			if(!(Character.isLetter(c) || c == '.' || c == ',' || c == '?' || c == '!' || c == ':' || c == ';')){
				return null;
			}/*Character next = word.charAt(i+1);
			if(next != null){
				if((c == '.' || c == ',' || c == '?' || c == '!' || c == ':' || c == ';') && next.isLetter(word.charAt(i+1))){
						return null;
				}
			}*/
		}
		/*char end = word.charAt(word.length()-1);	//check if the last char is an alphabetic character
		if(!(Character.isLetter(end) || end == '.' || end == ',' || end == '?' || end == '!' || end == ':' || end == ';')){
			return null;
		}*/
		for(int i = 0; i < word.length(); i++){ //remove punctuation
			char c = word.charAt(i);
			if(c == '.' || c == ',' || c == '?' || c == '!' || c == ':' || c == ';')
				word = word.substring(0,i);
		}
		word = word.toLowerCase();
		if(noiseWords.containsKey(word)){
			return null;
		}return word;
	}
	
	/**
	 * Inserts the last occurrence in the parameter list in the correct position in the
	 * same list, based on ordering occurrences on descending frequencies. The elements
	 * 0..n-2 in the list are already in the correct order. Insertion of the last element
	 * (the one at index n-1) is done by first finding the correct spot using binary search, 
	 * then inserting at that spot.
	 * 
	 * @param occs List of Occurrences
	 * @return Sequence of mid point indexes in the input list checked by the binary search process,
	 *         null if the size of the input list is 1. This returned array list is only used to test
	 *         your code - it is not used elsewhere in the program.
	 */
	public ArrayList<Integer> insertLastOccurrence(ArrayList<Occurrence> occs) {
		// COMPLETE THIS METHOD
		// THE FOLLOWING LINE HAS BEEN ADDED TO MAKE THE METHOD COMPILE
		ArrayList<Integer> frequencyList = new ArrayList<Integer>();
		int size = occs.size() - 1;
		for(int i = 0; i < size; i++) {frequencyList.add(occs.get(i).frequency);}
		int lastVal = occs.get(size).frequency; size = frequencyList.size()-1;
		ArrayList<Integer> list = binarySearch(frequencyList,0,occs.size() -2 ,lastVal);
		
		int lastMid = list.get(list.size()-1);
		if(list.get(list.size() -1) >occs.get(lastMid).frequency)
			occs.add(lastMid,occs.remove(occs.size()-1));
		else if(list.get(list.size() -1) < occs.get(lastMid).frequency){}		
		else occs.add(lastMid + 1, occs.remove(occs.size()-1));
		return list;
	}
	private ArrayList<Integer> binarySearch(ArrayList<Integer> bin, int binMin, int binMax, int key){
		ArrayList<Integer> midpoint = new ArrayList<Integer>();
		while(binMax >= binMin){
			int mid = (binMax + binMin) / 2;
			midpoint.add(mid);
			if(midpoint.get(mid) < key)
				binMax = mid - 1;
			else if(midpoint.get(mid) > key)
				binMin = mid + 1;
			else break;
		}
		return midpoint;
	}
	
	
	
	/*/*public static void main (String[] args){
		Scanner sc = new Scanner(System.in);
		System.out.println("word: ");
		String word = sc.next();
		
		word = reword(word);
		if(word == null){
			System.out.println("null");
		}else System.out.println(word);
		
	}
	public static String reword (String word){
		for(int i = 0; i*/
	/**
	 * Search result for "kw1 or kw2". A document is in the result set if kw1 or kw2 occurs in that
	 * document. Result set is arranged in descending order of occurrence frequencies. (Note that a
	 * matching document will only appear once in the result.) Ties in frequency values are broken
	 * in favor of the first keyword. (That is, if kw1 is in doc1 with frequency f1, and kw2 is in doc2
	 * also with the same frequency f1, then doc1 will appear before doc2 in the result. 
	 * The result set is limited to 5 entries. If there are no matching documents, the result is null.
	 * 
	 * @param kw1 First keyword
	 * @param kw1 Second keyword
	 * @return List of NAMES of documents in which either kw1 or kw2 occurs, arranged in descending order of
	 *         frequencies. The result size is limited to 5 documents. If there are no matching documents,
	 *         the result is null.
	 */
	public ArrayList<String> top5search(String kw1, String kw2) {
		// COMPLETE THIS METHOD
		// THE FOLLOWING LINE HAS BEEN ADDED TO MAKE THE METHOD COMPILE
		int doc1 = 0, doc2 = 0, count = 0;
		String wordA, wordB;
		ArrayList<Occurrence> doc1Occ = null;
		ArrayList<Occurrence> doc2Occ = null;
		ArrayList<String> searchResults = new ArrayList<String>(5); 	
		if (keywordsIndex.get(kw1) != null)
			doc1Occ = keywordsIndex.get(kw1);
		if (keywordsIndex.get(kw2) != null)
			doc2Occ = keywordsIndex.get(kw2);
		if((doc1Occ == null && doc2Occ == null)){
			return searchResults;
		}	
		else if (doc1Occ != null && doc2Occ == null)
		{
			int index = 0;
			while (searchResults.size() < 5 && index < doc1Occ.size())
			{
				if (!searchResults.contains(doc1Occ.get(index)))
				{
					searchResults.add(doc1Occ.get(index).document);
				}
				index++;
			}
		}
		else
		{
			int count1 = 0, count2 = 0;
			//traverse through both ArrayLists of the 2 keywords
			while (count1 < doc1Occ.size() && count2 < doc2Occ.size() && searchResults.size() < 5)
			{if (doc1Occ.get(count1).frequency > doc2Occ.get(count2).frequency)
				{
					if (!searchResults.contains(doc1Occ.get(count1).document))
					{
						searchResults.add(doc1Occ.get(count1).document);
					}count1++;
				}
				else if (doc1Occ.get(count1).frequency < doc2Occ.get(count2).frequency)
				{
					if (!searchResults.contains(doc2Occ.get(count2).document))
					{
						searchResults.add(doc2Occ.get(count2).document);
					}count2++;
				}
				else
				{if (!searchResults.contains(doc1Occ.get(count1).document))
						searchResults.add(doc1Occ.get(count1).document);
					if (searchResults.size() < 5 && !searchResults.contains(doc2Occ.get(count2).document))
					{
						searchResults.add(doc2Occ.get(count2).document);
					}count1++;count2++;
				}
			}
			if (count1 == doc1Occ.size())
			{
				while (count2 < doc2Occ.size() && searchResults.size() < 5)
				{
					if (!searchResults.contains(doc2Occ.get(count2).document))
					{
						searchResults.add(doc2Occ.get(count2).document);
					}count2++;
				}
			}
			if (count2 == doc2Occ.size())
			{
				while (count1 < doc1Occ.size() && searchResults.size() < 5)
				{
					if (!searchResults.contains(doc1Occ.get(count1).document))
					{
						searchResults.add(doc1Occ.get(count1).document);
					}count1++;
				}
			}
		}
		return searchResults;
	}	
	
	/* < word.length() - 1; i++){
			char c = word.charAt(i);
			if(!(Character.isLetter(c) || c == '.' || c == ',' || c == '?' || c == '!' || c == ':' || c == ';')){
				return null;
			}Character next = word.charAt(i+1);
			if(next != null){
				if((c == '.' || c == ',' || c == '?' || c == '!' || c == ':' || c == ';') && next.isLetter(word.charAt(i+1))){
						return null;
				}
			}
		}char end = word.charAt(word.length()-1);
		
		if(!(Character.isLetter(end) || end == '.' || end == ',' || end == '?' || end == '!' || end == ':' || end == ';')){
			return null;
		}
		
		for(int i = 0; i < word.length(); i++){
			char c = word.charAt(i);
			if(c == '.' || c == ',' || c == '?' || c == '!' || c == ':' || c == ';')
				word = word.substring(0,i);
		}
		
		word = word.toLowerCase();
		return word;
	}*/
}
