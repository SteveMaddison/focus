#include "focus/fifo.h"
#include "focus/shell.h"
#include "focus/shell/commands.h"
#include "focus/util.h"
#include "focus/memory.h"
#include "focus/vfs.h"
#include "focus/errno.h"
#ifdef ARCH_SPECTRUM
#include "focus/spectrum/console.h"
#endif

static int parse( char* buffer, char **words, int size );


int shell_start( void )
{
    char buffer[SHELL_BUFFER_SIZE];
    int fd, bytes, word_count, i = 0;
    char *words[SHELL_MAX_WORDS];

    fd = open( "con0:", 0 );
    if( fd == -1 ) {
    	printf("Can't open con0: (%d) %s\n", errno, strerr( errno ) );
    	return 1;
	}

    for(;;){
    	printf("> ");
		bytes = readline( fd, buffer, SHELL_BUFFER_SIZE );
		buffer[ bytes ] = '\0';
		if( word_count = parse( buffer, &words, bytes ) ) {
			if( !strcmp( words[0], "cls" ) ) {
				cls();
			}
			else if( !strcmp( words[0], "echo" ) ) {
				for( i = 1 ; i < word_count ; i++ ) {
					printf( "%s", words[ i ] );
					if( i <= word_count ) {
						printf(" ");
					}
				}
				printf("\n");
			}
			else if( !strcmp( words[0], "mem" ) ) {
				cmd_mem();
			}
			else {
				printf( "Unknown command\n" );
			}
		}
    }
}

static int parse( char *buffer, char *words[], int size )
{
	char *start = NULL;
	char *end = buffer + size;
	int count = 0;

	while( *buffer ) {
		while( *buffer && *buffer == ' ' )
			buffer++;
		start = buffer;

		words[count++] = start;

		while( *buffer && *buffer != ' ' )
			buffer++;

		*buffer = '\0';
		if( buffer < end )
			buffer++;
	}

	return count;
}
