class MedianFinder {//check lecture
    
    priority_queue<int, vector<int>, greater<int>> minHeap;   // min heap
    priority_queue<int> maxHeap;                              // max heap


public:
    
    void addNum(int num)// Adds a number into the data structure.
    {
        maxHeap.push(num);                                    // Add to max heap

        minHeap.push(maxHeap.top());                               // balancing step
        maxHeap.pop();

        if (maxHeap.size() < minHeap.size()) {                     // maintain size property
            maxHeap.push(minHeap.top());//pop top to adjust size and add to other heap
            minHeap.pop();
        }
    }

    // Returns the median of current data stream
    double findMedian()
    {
        return maxHeap.size() > minHeap.size() ? maxHeap.top() : ((double) maxHeap.top() + minHeap.top()) * 0.5;//returns dauble for halfs
    }
};