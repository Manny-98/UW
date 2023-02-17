/*
 * This is a template of QueueNode.java. Change this file name into QueueNode.java
 * and implement the logic.
 */

import java.util.*;

public class QueueNode {
    private Vector<Integer> tidQueue; // maintains a list of child TIDs who called wakeup( ).

    public QueueNode( ) {
	// Implement this constructor.
    tidQueue = new Vector<Integer>();
    }

    public synchronized int sleep( ) {
	// If tidQueue has nothing, call wait( ).
    if(tidQueue.size()==0){
        // wait() can throw an InterruptedException
        try{
            wait();
        }
        catch (InterruptedException e){
        }
    }
	// Otherwise, get one child TID from tidQueue.
	// return it.
        //tidQueue.get(0);
        int tid= tidQueue.remove(0);
        return tid;
    }

    public synchronized void wakeup( int tid ) {
	// Add this child TID to tidQueue.
    tidQueue.add(tid);
	// Notify the parent.
    notify();
    }
}
