/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 *
 */
#include <stdio.h>
#include <stdlib.h>
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


#define SIZE_T_SIZE (ALIGN(sizeof(size_t))) //var i tarna

/* Internal helper routine prototypes */
static void printblock(void *blockptr);

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
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

static void printblock(void *blockptr)
{ 
   printf("%p: ", blockptr); 
}
