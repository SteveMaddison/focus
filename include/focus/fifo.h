/*
 * fifo.h - Generic FIFO creation and handling
 *
 * (c)2003 Steve Maddison
 */

#ifndef _FIFO_H_
#define _FIFO_H_

#include "focus/types.h"

#define FIFO_EMPTY	0x01
#define FIFO_FULL	0x02


struct fifo_s {
	char	*head;
	char	*tail;
	char	*buffer;
	size_t	size;
	char	flags;
};

struct fifo_s *fifo_create( size_t size );
void fifo_destroy( struct fifo_s *fifo );
void fifo_clear( struct fifo_s *fifo );

int fifo_read( struct fifo_s *fifo, char *data, size_t length );
int fifo_write( struct fifo_s *fifo, char *data, size_t length );

#endif /* _FIFO_H_ */
