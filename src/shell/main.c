#include "focus/fifo.h"
#include "focus/shell.h"
#include "focus/shell/commands.h"
#include "focus/util.h"
#include "focus/memory.h"
#include "focus/vfs.h"
#include "focus/errno.h"


#include "focus/spectrum/keyboard.h"

static int parse( char* buffer, char **words, int size );


void shell_start( void )
{
    open( "con0:", 0 );
    for(;;){}
}

static int parse( char* buffer, char **words, int size )
{
	char separator = '\0';
	char *start = NULL;

	start = buffer;

	while( *buffer ) {
		if( *buffer == ' ' || *buffer == '\t' ) {
			while( *buffer == ' ' || *buffer == '\t' )
				buffer++;
			start = buffer;
		}

		if( *buffer == '"' || *buffer == '\'' ) {
			if( !separator ) {
				separator = *buffer++;
			}
			else {
				separator = '\0';
				*buffer++ = '\0';
				start = NULL;
			}
		}
		else {
			if( !start )
				start = buffer;
			buffer++;
		}
	}

	if( separator )
		return -1;

	return 0;
}

