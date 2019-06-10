/**
 * @Author Dan Engbert (end1@umbc.edu)
 * This file contains a program that defines function for allocating / deallocating memory
 *   and calls the external function hw4_test() to test these functions.
 *
 * Answers to part2:
 *  * How I track which frames are allocated:
 *      I have a struct Frame to store info about each frame and the type field
 *      is set to -1 if the frame has not been allocated yet.
 *      Otherwise it's set to 0 (if it's the first frame in a block), 1 (if
 *      it's the second frame in a block), 2 (if it's the third), ...
 *  * How I track how large a memory block is:
 *      The struct frame corresponding to the start of that memory block has
 *      its requested field set to the number of bytes allocated to the user.
 *
 *  1) If the user calls my_free() after first calling my_malloc(300), then
 *      I determine the number of frames belonging to the block with:
 *      int frameCount = ((aTable[f].requested + 2) / 64);
 *      if ((aTable[f].requested + 2) % 64 != 0)
 *          frameCount++;
 *
 *      For this example, frameCount would be calculated as 5,
 *      so I would deallocate frames f through f+4.
 *
 *  2) First I check the pointer passed to my_free() is equal to the address in
 *     mem at offset f * FRAME_SIZE for some particular int f in [0, NUM_FRAMES-1]
 *     (this ensures the address is the start of a frame).
 *     Then I verify that aTable[f].type == 0
 *     (this ensures that this frame was the first frame in an allocated block.
 *     If both these conditions aren't met then the pointer is in the middle of
 *     a memory block (or not in the memory block at all) so I raise a SEGFAULT.
 *
 * Help received:
 *   https://www.geeksforgeeks.org/program-next-fit-algorithm-memory-management/
 *   https://www.avrfreaks.net/forum/converting-uint16t-uint8t
 */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <inttypes.h>
#include <stdbool.h>

#define NUM_FRAMES 10
#define FRAME_SIZE 64
#define MEM_LENGTH (NUM_FRAMES * FRAME_SIZE)

// used by allocation table to store info about a specific frame
struct Frame {
    int type; // -1: frame not used, 0: frame is start of allocated block
              //  1: second frame in a block, 2: third frame in a block, ...
    unsigned int requested;  // number of bytes requested
    uint16_t canary;         // expected canary value
};

// global data:
char mem[MEM_LENGTH];             // 10 page frames, each holds 64 bytes
unsigned int frameStart = 0;      // next frame to search for space in (for next fit)
struct Frame aTable[NUM_FRAMES];  // allocation table
pthread_mutex_t data_mutex;       // mutex for global data

// function prototypes:
extern void hw4_test(void);       // defined elsewhere, tests functions implemented here
void my_malloc_stats(void);
void *my_malloc(size_t size);
void my_free(void *ptr);

int main(int argc, char *argv[]) {
    unsigned int seed = 0;
    if (argc == 2) {
        seed = atoi(argv[1]);
    }
    srand(seed);

    // ---initialize global data---
    memset(mem, 0, MEM_LENGTH); // initialize memory buffer to 0
    // setup allocation table
    for (int i=0; i<NUM_FRAMES; i++) {
        aTable[i].type = -1;
        aTable[i].requested = 0;
        aTable[i].canary = 0;
    }
    pthread_mutex_init(&data_mutex, NULL);      // initialize mutex

    // testing
    hw4_test(); // required to have this at end of my main()
    return 0;
}

/**
 * Write to standard output information about the current memory
 * allocations.
 *
 * Display to standard output the following:
 * - Memory contents, one frame per line, 10 lines total. Display the
 *   actual bytes stored in memory. If the byte is unprintable (ASCII
 *   value less than 32 or greater than 126), then display a dot
 *   instead.
 * - Current memory allocations. For each frame, display a 'f' if the
 *   frame is free, 'R' if reserved. If the frame is the beginning of a
 *   reserved memory block, display the four hexadecimal digit canary.
 *   Otherwise, display dashes instead of the canary.
 */
void my_malloc_stats(void) {
    pthread_mutex_lock(&data_mutex);
    printf("Memory contents:\n");
    for (int f=0; f<NUM_FRAMES; f++) {
        printf("  ");
        for (int i=0; i<FRAME_SIZE; i++) {
            int index = f * FRAME_SIZE + i;
            if (32 <= mem[index] && mem[index] <= 126)
                printf("%c", mem[index]);
            else
                printf(".");
        }
        printf("\n");
    }
    // current memory allocations
    printf("Memory allocation table:\n  ");
    for (int f=0; f<NUM_FRAMES; f++) {
        if (aTable[f].type == -1)
            printf("f:---- ");
        else if (aTable[f].type == 0)
            printf("R:%04x ", aTable[f].canary);
        else
            printf("R:---- ");
    }
    printf("\n");
    pthread_mutex_unlock(&data_mutex);
}

/**
 * Allocate and return a contiguous memory block that is within the
 * memory region.
 *
 * The size of the returned block will be at least @a size bytes,
 * rounded up to the next 64-byte increment.
 *
 * @param size Number of bytes to allocate. If @c 0, your code may do
 * whatever it wants; my_malloc() of @c 0 is "implementation defined",
 * meaning it is up to you if you want to return @c NULL, segfault,
 * whatever.
 *
 * @return Pointer to allocated memory, or @c NULL if no space could
 * be found. If out of memory, set errno to @c ENOMEM.
 */
void *my_malloc(size_t size) {
    if (size == 0)
        return NULL;
    int framesNeeded = (size+2) / 64;
    if ((size+2) % 64 != 0)
        framesNeeded++;
    int frame = frameStart;

    // find next available frame with sufficient space following it
    pthread_mutex_lock(&data_mutex);
    for (int offset=0; offset<NUM_FRAMES; offset++) {
        frame = (frameStart + offset) % NUM_FRAMES;
        if (aTable[frame].type == -1) {
            bool room = false; // enough room
            // check if there are *framesNeeded* sequential frames available:
            for (int cur=frame; cur<NUM_FRAMES; cur++) {
                if (aTable[cur].type != -1)
                    break;
                if (cur-frame + 1 == framesNeeded) {
                    room = true;
                    break;
                }
            }
            if (room)
                break;
        }

        if (offset == NUM_FRAMES - 1) {
            // no room in memory to allocate
            errno = ENOMEM;
            pthread_mutex_unlock(&data_mutex);
            return NULL;
        }
    }

    // allocate memory starting at *frame*
    uint16_t canary = (uint16_t) rand();
    for (int i=0; i<framesNeeded; i++) {
        aTable[frame + i].type = i;
        aTable[frame + i].requested = size; // we added 2 (for the canary) earlier
        aTable[frame + i].canary = canary;
    }
    // write canary to appropriate location
    size_t offset = (frame*FRAME_SIZE) + (size);
    mem[offset] = (uint8_t) (canary >> 8); // upper byte
    mem[offset+1] = (uint8_t) canary;      // lower byte
    frameStart = (frame + framesNeeded) % NUM_FRAMES;
    //printf("***just allocated %d bytes\n", (int) size);
    pthread_mutex_unlock(&data_mutex);
    return &(mem[frame*FRAME_SIZE]);
}

/**
 * Deallocate a memory region that was returned by my_malloc().
 *
 * If @a ptr is not a pointer returned by my_malloc(), then send a
 * SIGSEGV signal to the calling process. Likewise, calling my_free()
 * on a previously freed region results in a SIGSEGV.
 *
 * @param ptr Pointer to memory region to free. If @c NULL, do
 * nothing.
 */
void my_free(void *ptr) {
    pthread_mutex_lock(&data_mutex);
    for (int f=0; f<NUM_FRAMES; f++) {
        if (&(mem[f*FRAME_SIZE]) == ptr && aTable[f].type == 0) {
            // (pointer is valid so free memory)
            int frameCount = ((aTable[f].requested + 2) / 64);
            if ((aTable[f].requested + 2) % 64 != 0)
                frameCount++;

            // check that user didn't overwrite the canary (SIGUSR1)
            uint8_t e1 = (uint8_t) (aTable[f].canary >> 8), e2 = (uint8_t) aTable[f].canary;
            char *c = &(mem[f*FRAME_SIZE + aTable[f].requested]);
            uint8_t c1 = (uint8_t) *c, c2 = (uint8_t) *(c+1);
            bool changed = (c1 != e1) || (c2 != e2); // whether canary was changed
            //printf("comparing %02x to %02x and %02x to %02x -> %d\n", c1, e1, c2, e2, changed);

            // deallocate memory
            for (int offset=0; offset<frameCount; offset++) {
                int frame = f + offset;
                aTable[frame].type = -1;
                aTable[frame].requested = 0;
                aTable[frame].canary = 0;
            }
            pthread_mutex_unlock(&data_mutex);
            if (changed) {
                printf("changed = true\n");
                raise(SIGUSR1);
            }
            return;
        }
    }
    pthread_mutex_unlock(&data_mutex);
    raise(SIGSEGV);
}
