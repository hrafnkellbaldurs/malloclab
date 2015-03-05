/*
 * 
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * In this explicit list implementation, we will use a doubly linked list
 */
#include <stdio.h>
#include <stdlib.h>

#define NDEBUG //ignores assert calls
#include <assert.h>

#include <unistd.h>
#include <string.h>
#include "mm.h"
#include "memlib.h"

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

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define BLK_HDR_SIZE ALIGN(sizeof(blockHdr))

typedef struct header blockHdr;

struct header {
	size_t size;
	blockHdr *next_p;
	blockHdr *prev_p;
};


/* Internal helper routine prototypes */
//static void printblock(void *blockptr);
void print_heap();
void *find_fit(size_t size);

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
   //makes the iirst free block
   blockHdr *p = mem_sbrk(BLK_HDR_SIZE);
   p->size = BLK_HDR_SIZE;
   p->next_p = p;
   p->prev_p = p;
    
    /*
    
    bool problem = false;
    	
    allocate inital heap area

    if(initializing fails)
    {
	return -1;
    }
    
   */

    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    //every block has a header whether its allocated or free
    //we probably want to eliminate the header and just saving a header 
    //word for our allocator blocks
    int newsize = ALIGN(size + BLK_HDR_SIZE);
    blockHdr *bp = find_fit(newsize);
    //if we didnt find anything in the list
    if(bp == NULL){
        bp = mem_sbrk(newsize);
	if (bp == (void *)-1)
	    return NULL;
	else{
	    bp->size = newsize | 1;
	}	
    
    } else {
	//basic linked list removal
	bp->size |= 1;
	bp->prev_p->next_p = bp->next_p;
	bp->next_p->prev_p = bp->prev_p;
    }
    return (char *)bp + BLK_HDR_SIZE;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{ 
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

    /*	
    //if ptr is null, the call is equivalent to mm_malloc(size), NULL
    if(ptr == NULL)
      return mm_malloc(size);
    //if ptr is not NULL, it must have been returned by an earlier call to mm    _malloc or mm_realloc
       
    //if size is equal to zero, the call is equivalent to mm_free(ptr)
    if(size == 0)
      return mm_free(ptr);
    */

    return 0;
}

/* Internal helper routines */

void *find_fit(size_t size)
{
   blockHdr *p;
   for (p = ((blockHdr *)mem_heap_lo())->next_p;
	p != mem_heap_lo() && p->size < size;
	p = p->next_p);

   //as long as I found something that isn't the first block
   if(p != mem_heap_lo())
	return p;
   else
	return NULL;	
   return 0;
}

void print_heap(){
   blockHdr *bp = mem_heap_lo();
   while(bp < (blockHdr *)mem_heap_hi()) {
   	printf("%s block at %p, size %d\n", 
		(bp->size&1)?"allocated":"free",
		bp, (int)(bp->size & ~1));
	bp = (blockHdr *)((char *)bp + (bp->size & ~1));
   }
}

//static void printblock(void *blockptr)
//{ 
//   printf("%p: ", blockptr); 
//}
