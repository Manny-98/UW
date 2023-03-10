#include <unistd.h> // sbrk( )
#include <limits.h> // LONG_MAX

using namespace std;

static bool initialized = false;
static void *heap_top; // the beginning of the heap space
static void *heap_end; // the current boundary of the heap space, obtained from sbrk( 0 )

class MCB
{ // memory control block
public:
  int available; // true(1): this memory partition is available, false(0) unavailalbe.
  int size;      // MCB size + the user data size
};

void free_(void *dealloc_space)
{
  MCB *mcb;

  // locate this partition's mcb address from dealloc_space
  mcb = (MCB *)((unsigned long long int)dealloc_space - sizeof(MCB));
  // Task 4: implement by yourself (just in one line).
  mcb->available = true;
  return;
}

void *malloc_f(long size)
{
  struct MCB *cur_mcb;    // current MCB
  void *new_space = NULL; // this is a pointer to a new memory space allocated for a user

  if (!initialized)
  {
    // find the end of heap memory, upon an initialization
    heap_end = sbrk(0);
    heap_top = heap_end;
    initialized = true;
  }

  // append an MCB in front of a requested memroy space
  size = size + sizeof(MCB);

  // Task 1: implement by yourself (up to 15 lines).
  // scan each mcb from the top to the bottom of the heap
  for (void *cur = heap_top; cur < heap_end; cur = (void *)((unsigned long long int)cur + cur_mcb->size))
  {
    cur_mcb = (MCB *)cur;
    //   if cur_mcb->available and cur_mcb->size fits size, new_space points to this mcb
    if (cur_mcb->available && cur_mcb->size >= size)
    {
      cur_mcb->available = 0;
      //   let cur_mcb point to each mcb you are scanning
      new_space = cur_mcb;
      break;
    }
  }

  // Task 2: implement by yourself (up to 5 lines).
  // no space found yet
  if (new_space == NULL)
  {
    // get a space from OS
    sbrk(size);
    // old boundary now becomes new_space, i.e. initialize new_space with heap_end
    new_space = heap_end;
    // heap_end will go down by size
    heap_end = (void *)((unsigned long long int)heap_end + size);
    // initialize cur_mcb with new_space and size.
    cur_mcb = (MCB *)new_space;
    cur_mcb->available = 0;
    cur_mcb->size = size;
  }

  // new space is after new MCB
  return (void *)((long long int)new_space + sizeof(MCB));
}

void *malloc_b(long size)
{
  struct MCB *cur_mcb;    // current MCB
  void *new_space = NULL; // this is a pointer to a new memory space allocated for a user

  if (!initialized)
  {
    // find the end of heap memory, upon an initialization
    heap_end = sbrk(0);
    heap_top = heap_end;
    initialized = true;
  }

  // append an MCB in front of a requested memroy space
  size = size + sizeof(MCB);
  // temps to save mcb and size
  MCB *currentBest;
  int bestSize = 0;
  // Task 3: implement by yourself (up to 20 lines).
  // scan each mcb from the top to the bottom of the heap
  for (void *cur = heap_top; cur < heap_end; cur = (void *)((unsigned long long int)cur + cur_mcb->size))
  {
    //   let cur_mcb point to each mcb you are scanning
    cur_mcb = (MCB *)cur;
    //   if cur_mcb->available and cur_mcb->size fits size and cur_mcb->size is the best size so far
    //   temporarily memorize this best size so far and this best mcb so far
    if (cur_mcb->available && cur_mcb->size >= size)
    {
      if (cur_mcb->size == size)
      {
        currentBest = cur_mcb;
        bestSize = cur_mcb->size;
        break;
      }

      if (bestSize == 0 || cur_mcb->size <= bestSize)
      {
        currentBest = cur_mcb;
        bestSize = cur_mcb->size;
      }
    }
  }
  //   After scan, check the best mcb so far. If it is not null
  if (currentBest != NULL)
  {
    // make it unavailable as its filled now
    currentBest->available = 0;

    // this space can have current best
    //    new_space points to this best mcb so rar
    new_space = currentBest;
  }

  // no space found yet
  if (new_space == NULL)
  {
    // The same as Task 2
    // Task 3': Just cut and past the logic from malloc_f to here.

    // get a space from OS
    sbrk(size);
    // old boundary now becomes new_space, i.e. initialize new_space with heap_end
    new_space = heap_end;
    // heap_end will go down by size
    heap_end = (void *)((unsigned long long int)heap_end + size);
    // initialize cur_mcb with new_space and size.
    cur_mcb = (MCB *)new_space;
    cur_mcb->available = 0;
    cur_mcb->size = size;
  }

  // new space is after new MCB
  return (void *)((long long int)new_space + sizeof(MCB));
}
