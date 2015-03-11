/*
*  We'll be using an explicit free list implementation with a 
*  doubly linked list traversal through the free list. The free
*  block scanning alghorithm will be Best Fit.
*
*  Malloc:
*	 We use the Best Fit alghorithm to find a fitting free block 
*	 of the least size possible
*  Free:
*	 We'll be clearing the payload for the allocated block 
*	 requested to be freed. Then we mark the block as free and 
*	 perform coalescing. 
*	 To make the most of the free blocks, we will be using 
*	 immediate coalescing to take care of external fragmentation 
*	 that may occur. Two pointers to the beginning and end of the 
*	 free list are maintained throughout
*
*  How our blocks will look like:
*
*  Free block:        HEADER |  |PREV *| NEXT*|  | FOOTER
*  Allocated block:   HEADER | ----PAYLOAD-----  | FOOTER
*
*  In the header and footer: 
*  8 byte word containing the size of the block, allocated flag 
*  bit and reallocated flag bit
*
*  Looks like this:
*
*   31                     3  2  1  0 
*   -----------------------------------
*  | s  s  s  s  ... s  s  s  0  r  a/f
*   -----------------------------------
*
*   r is set iff the block has bees reallocated.
*   a/f is set iff the block is allocated 
*/

#include <stdio.h>
#include <stdlib.h>

//#define NDEBUG //ignores assert calls
#include <assert.h>

#include <unistd.h>
#include <string.h>
#include "mm.h"
#include "memlib.h"

extern int verbose;

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in below _AND_ in the
 * struct that follows.
 *
 * === User information ===
 * Group: The_Memory_Allogators
 * User 1: hrafnkell13
 * SSN: 3012932249
 * User 2: arnara13
 * SSN: 0608893339
 * === End User Information ===
 ********************************************************/
team_t team = {
    /* Group name */
    "The Memory Allogators",
    /* First member's full name */
    "Hrafnkell Baldursson",
    /* First member's email address */
    "hrafnkell13@ru.is",
    /* Second member's full name (leave blank if none) */
    "Arnar Dori Asgeirsson",
    /* Second member's email address (leave blank if none) */
    "arnara13@ru.is",
    /* Leave blank */
    "",
    /* Leave blank */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* pointer casting maicros */
#define GEN_P(bp) (void *)(bp)

/* combine size and allocated flag bit into a 4 byte size_t */
#define PACK(size, alloc) ((size) | (alloc))

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

/* rounds up to the size of a size_t to the nearest multiple of ALIGNMENT */
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* round up to the size of a block_hdr(12 bytes)  */
#define BLK_HDR_SIZE ALIGN(sizeof(block_hdr))

/* read and write a word at address p */
#define GET(p)       (*(size_t *)(p))
#define PUT(p, val)  (*(size_t *)(p) = (val))

#define GET_SIZE(p)  (p->size) //(GET(p) & ~0x7)
//#define GET_ALLOC(p)  (GET(p) & 0x1) SKIL EKKI

/* given a header pointer bp, compute address of footer */
#define FTRP(bp)  ((char *)(bp) + ALIGN) //UNDER CONSTRUCTION

#define DEBUG

#define CHECK_HEAP(verbose) printf("%s\n", __func__); mm_checkheap(verbose);

#ifdef DEBUG
#define CHECK CHECK_HEAP(verbose)
#endif

typedef struct header block_hdr;

// header for doubly linked list
struct header {
        size_t size;
        block_hdr *next_p;
        block_hdr *prev_p;
};


/* Internal helper routine prototypes */
//static void printblock(void *blockptr);
static void *find_fit(size_t size);
static void *coalesce(void *bp);
static int mm_checkheap(int verbose);
static void print_heap();


/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{   
  //expands the heap by 16 bytes, or the size of block_hdr + footer
  //and makes our first block point to the first byte of 
  //the allocated heap area
  block_hdr *bp = mem_sbrk(BLK_HDR_SIZE + ALIGNMENT);
  bp->size = BLK_HDR_SIZE + ALIGNMENT;
  bp->next_p = NULL;
  bp->prev_p = NULL;

  //TODO: SET SIZE OF BLOCK TO FOOTER

  //delete this later
  //bp->size = bp->size | 1; //sets it as allocated

  CHECK;

  //if our initial block goes past the heap, we have a problem
  if(GEN_P(bp) > mem_heap_hi())
	return -1;

    return 0;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
   //we need the struct size plus the actual info
   //TODO: eliminating header struct in allocated blocks
   //int newsize = ALIGN(BLK_HDR_SIZE + size);   

    int newsize = ALIGN(size + SIZE_T_SIZE);
    void *p = mem_sbrk(newsize);
    if (p == (void *)-1)
        return NULL;
    else {
        *(size_t *)p = size;
        return (void *)((char *)p + SIZE_T_SIZE);
    } 

}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{ 
   //bullshit code to ignore warnings
   block_hdr *bp = coalesce(ptr);
   bp = find_fit(BLK_HDR_SIZE);
   
/*
   free the block pointed to by prt, something like this:
   free_block(ptr)

  */
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;

    newptr = mm_malloc(size);

    /*
    //if ptr is null, the call is equivalent to mm_malloc(size), NULL
    if(ptr == NULL)
      return mm_malloc(size);
    //if ptr is not NULL, it must have been returned by an earlier call to mm    _malloc or mm_realloc

    //if size is equal to zero, the call is equivalent to mm_free(ptr)
    if(size == 0)
      return mm_free(ptr);
    */

    if (newptr == NULL)
        return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
        copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}


/* Internal helper routines */

//find the next fitting free block for allocation
static void *find_fit(size_t size){
	return NULL;
}

//bind together two free blocks into one
static void *coalesce(void *bp){
	return NULL;
}

//checks the heap for inconsistancy
static int mm_checkheap(int verbose){
	/*
	Examples of what a heap checker might check are:

	Is every block in the free list marked as free?

	Are there any contiguous free blocks that somehow escaped coalescing?

	Is every free block actually in the free list?

	Do the pointers in the free list point to valid free blocks?

	Do any allocated blocks overlap?

	Do the pointers in a heap block point to valid heap addresses?
	
	returns nonzero iff heap is consistent
			else returns zero
	*/
	
	// print heap and dont check too much
	if(verbose == 0){
		print_heap();
	} 
	else if(verbose == 1){
	 // be verbose
	}
	else if(verbose == 2) {
	//be even more verbose
	}

	return 0;
}

/* from the beginning of the heap to the end, print each block and its information */
static void print_heap(){
	block_hdr *bp = mem_heap_lo();
	//taking advantage of the size, we can iterate through the blocks
	while(bp < (block_hdr *)mem_heap_hi()){
		printf("%s block at %p, size %d\n", 
		 	(bp->size&1)?"allocated":"free",
			 bp,
			 (int)(bp->size & ~1));
		bp = (block_hdr *)(char *)bp + (bp->size & ~1);
		//the ~1 is so we dont get the a bit with the size
	}   
}



//static void printblock(void *blockptr)
//{
//   printf("%p: ", blockptr);
//}

