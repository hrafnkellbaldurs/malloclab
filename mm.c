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
*  8 bit word containing the size of the block, allocated flag 
*  bit.
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

#define BLK_HDR_SIZE ALIGN(sizeof(block_hdr))

#ifdef DEBUG
#define CHECKHEAP(verbose) printf("%s\n", __func__); mm_checkheap(verbose);
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
void print_heap();
void mm_checkheap(char flag);

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
  //núllstilla breytur

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


void mm_checkheap(char flag){
    
}

/* from the beginning of the heap to the end, print each block and its information */
void print_heap(){
   
}


//static void printblock(void *blockptr)
//{
//   printf("%p: ", blockptr);
//}

