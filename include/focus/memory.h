#ifndef _MEMORY_H_
#define _MEMORY_H_

/* constants for kmalloc flags */
#define MEM_FREE	0x00
#define MEM_USED	0x01
#define MEM_KERNEL	0x02

#include "focus/types.h"

struct mblock_s {
	void	*base;
	char	flags;
	size_t	size;	/* excluding header */
	struct mblock_s	*prev;
	struct mblock_s	*next;
};

struct mem_stat_s {
	size_t	total;
	size_t	free;
	size_t	used;
};

#define HEADER_SIZE (sizeof(struct mblock_s))

extern int memory_init( void );
extern void* kmalloc( size_t size, char flags );
extern void kfree( void* ptr );
extern struct mem_stat_s *get_mem_stats( void );
extern void memory_map( void );

#endif /* _MEMORY_H_ */

