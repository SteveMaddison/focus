/*
 * fifo.c - Generic FIFO creation and handling
 *
 * (c)2003 Steve Maddison
 */

#include "focus/fifo.h"
#include "focus/memory.h"

struct fifo_s *fifo_create( size_t size )
{
	if ( !size ) {
		return NULL;
	}
	else {
		struct fifo_s* new_fifo;

		new_fifo = (struct fifo_s*)kmalloc( sizeof( struct fifo_s ) + size, MEM_KERNEL );
		if ( !new_fifo )
			return NULL;

		/* point to buffer directly after struct */
		new_fifo->buffer	= (char*)new_fifo + sizeof( struct fifo_s );
		new_fifo->head		= new_fifo->buffer;
		new_fifo->tail		= new_fifo->buffer;
		new_fifo->size		= size;
		new_fifo->flags		= FIFO_EMPTY;

		return new_fifo;
	}
}

void fifo_destroy( struct fifo_s *fifo ) {
	kfree( fifo );
}

void fifo_clear( struct fifo_s *fifo ) {
	fifo->head	= fifo->buffer;
	fifo->tail	= fifo->buffer;
	fifo->flags	= FIFO_EMPTY;
}

int fifo_read( struct fifo_s *fifo, char *data, size_t length )
{
	if ( fifo->flags & FIFO_EMPTY ) {
		return 0;
	}
	else {
		size_t count = 0;
		char *fifo_end = fifo->buffer + fifo->size;

		while ( count < length ) {
			*(data + count++) = *(fifo->head++);
			if ( fifo->head > fifo_end ) {
				fifo->head = fifo->buffer;
			}
			if ( fifo->head == fifo->tail ) {
				fifo->flags = FIFO_EMPTY;
				break;
			}
		}

		if( count )
			fifo->flags &= ~FIFO_FULL;

		return count;
	}
}

int fifo_write( struct fifo_s *fifo, char *data, size_t length )
{
	if ( fifo->flags & FIFO_FULL ) {
		return 0;
	}
	else {
		size_t count = 0;
		char *fifo_end = fifo->buffer + fifo->size;

		while ( count < length ) {
			*(fifo->tail++) = *(data + count++);
			if ( fifo->tail > fifo_end ) {
				fifo->tail = fifo->buffer;
			}
			if ( fifo->tail == fifo->head ) {
				fifo->flags = FIFO_FULL;
				break;
			}
		}

		if( count )
			fifo->flags &= ~FIFO_EMPTY;

		return count;
	}
}
