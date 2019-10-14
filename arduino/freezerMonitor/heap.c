/*
Simple minheap data structure of limited size
Arduino uses 2 byte ints and 4 byte longs
*/


struct measurement {
  unsigned long timestamp;
  int value;
};

#define MAX_CAP 1 << 8

void printTree(int level);


int capacity = -1;
struct measurement heap[MAX_CAP];

void heapifyDown(int root);
void heapifyUp(int child);
void swapValues(int a, int b);

void replace(unsigned long timestamp, int value) {
  // ensure the operation should go on
  if(heap[0].value > value) { 
    return;
  }

  // perform insert
  heap[0].value = value;
  heap[0].timestamp = timestamp;

  // heapify down
  heapifyDown(0);
}

int peek() {
  return heap[0].value;
}

int insert(unsigned long timestamp, int value) {
  if(capacity == MAX_CAP-1) {
    replace(timestamp, value);
  } else {
    capacity++;
    heap[capacity].value = value;
    heap[capacity].timestamp = timestamp;
    heapifyUp(capacity);
  }
  return MAX_CAP - capacity - 1;
}

// populates a measurement pointer, should never be null!
// returns number of items remaining (stop at 0!)
int pop(struct measurement *retVal) {
  if(capacity >= 0) {
    retVal->value = heap[0].value;
    retVal->timestamp = heap[0].timestamp;
    if(capacity != 0) {
      swapValues(0, capacity);
      capacity--;
      heapifyDown(0);
    } else {
      capacity--;
    }
  }
  return capacity+1;
}

void heapifyDown(int parent) {
  // end condition -- has no children
  int leftChild = 2*parent + 1;
  if(leftChild > capacity) {
    return;
  }

  // right child exists and is MIN of children
  if(leftChild+1 <= capacity && heap[leftChild+1].value < heap[leftChild].value) {
    if(heap[parent].value > heap[leftChild+1].value) {
      // right child should be updated and continue heapifying down
      swapValues(parent, leftChild+1);
      heapifyDown(leftChild+1);
    }
  }
  else if(heap[parent].value > heap[leftChild].value) {
    // left child is min and should be swapped -- then heapify down left child
    swapValues(parent, leftChild);
    heapifyDown(leftChild);
  }
  // end condition -- no more swapping was needed, order reestablished
  return;
}

void heapifyUp(int vertex) {
  // end condition -- has no parent (is root!)
  if(vertex == 0) {
    return;
  }
  int parent = vertex-1 >> 1;

  // only need check the one parent, the sibling is ordered
  if(heap[vertex].value < heap[parent].value) {
    // continue heapify up if swap occurs
    swapValues(vertex, parent);
    heapifyUp(parent);
  } 
  // end condition -- no more swapping was needed, order reestablished
  return;
}

// a and b should be defined and within the bound of heap
void swapValues(int a, int b) {
  int tempInt;
  unsigned long tempLong;
  tempLong = heap[a].timestamp;
  tempInt = heap[a].value;
  heap[a].timestamp = heap[b].timestamp;
  heap[a].value = heap[b].value;
  heap[b].timestamp = tempLong;
  heap[b].value = tempInt;
}

