#include "focus/util.h"
#include "focus/errno.h"


/* This happens when something goes
 * particularly horribly wrong...
 */
void doh( const char* message )
{
	printf( ", ,\n0o0 D'oh! (%d)\n ~\n%s\n", errno, message );
	stack_dump();

	/* it's not like we can do a lot about it now... */
	for (;;);
}

void stack_dump( void )
{
	char* sp = get_sp();
	size_t size = KSTACK_END-(size_t)sp + 1;
	char* loc;

	printf( "stack dump:\n" );

	for ( loc = (char*)KSTACK_END - 1 ; loc >= sp ; loc -= 2 ) {
		printf( "%x: %x", loc, (size_t)*loc );
		if ( loc == sp )
			printf( "<SP" );
		printf("\n");
	}
}

void *memcpy( void *dest, const void *src, size_t n )
{
	char *d = dest;
	char *s = src;
	size_t i;

	for ( i = 0 ; i < n ; i++ )
		*d++ = *s++;

	return dest;
}

void *memset( void *dest, char byte, size_t n )
{
	size_t i;
	char* d = dest;

	for( i = 0 ; i < n ; i++ )
		*d++ = byte;

	return dest;
}

char *strcpy( char* dest, const char* src )
{
	while( (*dest++ = *src++) );

	return dest;
}

int strcmp( const char* this, const char* that ) {
	while ( *this && *that ) {
		if ( *this++ != *that++ ) {
			return 1;
		}
	}
	if ( *this || *that )
		return 1;
	else
		return 0;
}

int strlen( const char* string )
{
	int length = 0;
	while( string++ != '\0' )
		length++;

	return length;
}
