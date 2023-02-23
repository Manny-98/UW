/*
 * This is a template of SyncQueue.java. Chagne this file name into SyncQueue.java and
 * complete the implementation
 */
public class SyncQueue {
    private QueueNode queue[] = null;
    private final int COND_MAX = 10;
    private final int NO_TID = -1;

    public SyncQueue( ) {
	// You need to implement this constructor.
	// Assume SyncQueue( COND_MAX );
    construct(COND_MAX);
    }

    public SyncQueue( int condMax ) {
	// You need to implement this constructor.
    construct(condMax);
    }

    public void construct(int number){
        queue = new QueueNode[number];
        for(int i =0;i < number;i++){
            queue[i] = new QueueNode();
        }
    }

    int enqueueAndSleep( int condition ) {
	// Verify the correctness of condition.
    if(condition >= 0 && condition < queue.length) {
	// Call the corresponding queue[ ].sleep( ).
    // Return the corresponding child thread ID.
    return queue[condition].sleep();
    }
     else {
	    //-1 if invalid 
	    return -1;
	}
    }

    void dequeueAndWakeup( int condition, int tid ) {
	// verify the correctness of condition.
    if(condition >= 0 && condition < queue.length) {
	// Call the corresponding queue[ ].wakeup( ... );
    queue[condition].wakeup(tid);
    }
    }

    void dequeueAndWakeup( int condition ) {
	// Assume tid = 0.
    dequeueAndWakeup(condition, 0);
    }
}
