#include "focus/fifo.h"
#include "focus/shell.h"
#include "focus/shell/commands.h"
#include "focus/shell/env.h"
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
    char *prompt;

    fd = open( "con0:", 0 );
    if( fd == -1 ) {
    	printf("Can't open con0: (%d) %s\n", errno, strerr( errno ) );
    	return 1;
	}

	env_init();

    for(;;){
    	prompt = env_get( "PROMPT" );
    	if( !prompt ){
    		prompt = "> ";
    	}
    	printf("%s", prompt);

		bytes = readline( fd, buffer, SHELL_BUFFER_SIZE );
		buffer[ bytes ] = '\0';
		word_count = parse( buffer, &words, bytes );
		if( word_count ) {
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
			else if( !strcmp( words[0], "env" ) ) {
				cmd_env( word_count, words );
			}
			else if( !strcmp( words[0], "exit" ) ) {
				printf("bye...\n");
				return 0;
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
	char *pos = buffer;
	char *end = buffer + size;
	char quote;
	int count = 0;

	while( *pos ) {
		while( *pos && *pos == ' ' )
			pos++;
		start = pos;

		switch( *pos ) {
			case '"':
			case '\'':
				quote = *pos;
				start++;
				pos++;
				while( *pos && *pos != quote ) {
					pos++;
				}
				if( *pos == quote ) {
					*pos = '\0';
					pos++;
				}
				else {
					printf( "Unmatched quote (%c)\n", quote );
					return 0;
				}
				words[count++] = start;
				break;

			case '$':
				start++;
				pos++;
				while( *pos && *pos != ' ' ) {
					pos++;
				}
				*pos = '\0';
				if( words[count] = env_get( start ) )
					count++;
				break;

			default:
				words[count++] = start;
				break;
		}

		while( *pos && *pos != ' ' )
			pos++;

		*pos = '\0';
		if( pos < end )
			pos++;
	}

	return count;
}
