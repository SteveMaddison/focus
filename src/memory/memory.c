#include "config.h"
#include "focus/memory.h"
#include "focus/errno.h"
#include "focus/types.h"
#include "focus/util.h"

/* start and end of doubly-linked list of blocks */
static struct mblock_s *start;
static struct mblock_s *end;

/* Total and free RAM is counted including space used for headers.
 * Although this seems a bit dishonest at first, joining blocks will
 * make memory used for n-1 blocks available for data, whereas
 * splitting blocks will claim memory to make new headers.
 */
struct mem_stat_s mem_stats;

/* Here we set a bogus variable which, as long as this object file
 * is linked as last in the list, will point to right after the static
 * kernel variables.
 */
static char bogus;
#define URAM_START ((size_t)&bogus)

static void *try_split_block( struct mblock_s *block, size_t size );
static void *try_join_blocks( size_t size );


int memory_init( void )
{
	size_t block_size = MEM_BLOCK_LARGEST;
	size_t block_start = KSTACK_START;
	char i;
	struct mblock_s *block = NULL;
	struct mblock_s *next_block = NULL;

	mem_stats.total = 0;

	/* starting in high memory with largest block size, initialise the blocks */
	while ( block_start >= URAM_START + MEM_BLOCK_SMALLEST ) {
		for ( i = MEM_BLOCK_BLOCKS ; i > 0 ; i-- ) {
			block_start -= block_size;
			if ( block_start < URAM_START ) {
				/* no room for another block */
				break;
			}
			else {
				block = (struct mblock_s*)block_start;
				block->base		= (void*)(block_start + HEADER_SIZE);
				block->size		= block_size - HEADER_SIZE;
				block->flags	= MEM_FREE;
				block->next		= next_block;
				if ( next_block )
					next_block->prev = block;
				else
					end = block;

				next_block = block; /* save for next iteration */
				mem_stats.total += block_size;
			}
		}
		block_size >>= 1;
		/* The following allows us to fill any remaining memory with blocks of
		 * MEM_BLOCK_SMALLEST bytes after the main groups of blocks are made.
		 */
		if ( block_size < MEM_BLOCK_SMALLEST )
			block_size = MEM_BLOCK_SMALLEST;
	}
	/* this will always be the first block in list */
	start = block;
	block->prev = NULL;

	mem_stats.free = mem_stats.total;
	printf( "%dKb RAM\n", mem_stats.total / 1024 );

	return 0;
}

struct mem_stat_s *get_mem_stats( void )
{
	return &mem_stats;
}

void* kmalloc( size_t size, char flags )
{
	struct mblock_s *block = start;

	/* find the smallest block into which out data will fit */
	while( block ) {
		if ( block->flags == MEM_FREE && block->size >= size ) {
			block = try_split_block( block, size );
			break;
		}
		block = block->next;
	}

	/* if that didn't work, try to merge blocks to create one large enough */
	if( ! block ) {
		block = try_join_blocks( size );
	}

	if( block ) {
		block->flags = MEM_USED | flags;
		mem_stats.used += size;
		mem_stats.free -= (block->size + HEADER_SIZE);
		return block->base;
	}

	errno = ENOMEM;
	return NULL;
}

void kfree( void* ptr )
{
	if( !ptr ) {
		return;
	}
	else {
		struct mblock_s *block = start;

		while( block ) {
			if( block->base == ptr ) {
				block->flags = MEM_FREE;
				mem_stats.used -= block->size;
				mem_stats.free += (block->size + HEADER_SIZE);
				return;
			}
			block = block->next;
		}
		/* XXX - The pointer didn't belong to any of our
		 * blocks - should we report a warning here?
		 */
	}
}

/* Split a large block into two. This works recursively, ending
 * when splitting the block would make it either too small for
 * the data, or smaller than the smallest block allowed.
 */
static void *try_split_block( struct mblock_s *block, size_t size )
{
	struct mblock_s *new_block;

	while ( (block->size - HEADER_SIZE) / 2 >= size &&
			(block->size + HEADER_SIZE != MEM_BLOCK_SMALLEST) ) {

		/* make two blocks of the same size */
		block->size = (block->size - HEADER_SIZE) / 2;
		/* this next step done indirectly to avoid bogus pointer arithmetic */
		new_block = block;
		new_block += block->size + HEADER_SIZE;
		new_block->size = block->size;

		/* copy over any attributes */
		new_block->flags = block->flags;

		/* update pointers and insert the new block into the list after the current one */
		new_block->base = (void*)(new_block + HEADER_SIZE);
		new_block->next = block->next;
		new_block->prev = block;
		if(	block->next )
			block->next->prev = new_block;
		else
			end = new_block;
		block->next = new_block;
	}

	return block;
}

/* Attempt to find consecutive blocks we can join to form one _valid_
 * block of at least <size> bytes. Note that our new block may exceed
 * MEM_BLOCK_LARGEST!
 * Instead of working recursively, it just searches for a long enough
 * chain of free blocks.
 */
static void *try_join_blocks( size_t size )
{
	size_t req_size = MEM_BLOCK_SMALLEST;

	/* determine a valid block size into which the data could fit */
	while( size + HEADER_SIZE > req_size )
		req_size <<= 1;

	/* starting with (hopefully) the largest blocks, see if we can fit this in */
	{
		struct mblock_s *block = end;
		struct mblock_s *last_block = NULL;
		size_t contig = 0;

		while( block ) {
			/* Mr Humphreys - are you free? */
			if( block->flags == MEM_FREE ) {
				if( !last_block ) {
					last_block = block;
				}

				contig += block->size + HEADER_SIZE;
				if( contig >= req_size ) {
					/* check the contiguous space can form a valid block */
					size_t check;
					for( check = MEM_BLOCK_SMALLEST ; check <= mem_stats.total ; check <<= 1 ) {
						if( contig == check )
							break; /* only out of for loop */
					}
					if ( check <= mem_stats.total )
						break; /* out of while loop, we have space! */
				}
			}
			else {
				/* end of chain of free blocks */
				contig = 0;
				last_block = NULL;
			}

			block = block->prev;
		}

		if( block ) {
			/* merge the whole lot into one block */
			block->size = (contig - HEADER_SIZE);
			block->next = last_block->next;
			if( last_block->next )
				last_block->next->prev = block;
			else
				end = block;
		}

		return block;
	}
}

/* This prints out a list of all blocks. */
void memory_map( void )
{
	struct mblock_s *block = start;

	while( block ) {
		printf(
			"@%x-%x:%x %d f=%x > %x\n",
			(size_t)block,
			(size_t)block + (block->size + HEADER_SIZE - 1),
			block->base,
			block->size + HEADER_SIZE,
			block->flags,
			block->next
		);

		block = block->next;
	}
}
