#include "focus/fifo.h"
#include "focus/shell.h"
#include "focus/shell/commands.h"
#include "focus/shell/env.h"
#include "focus/util.h"
#include "focus/memory.h"
#include "focus/vfs.h"
#include "focus/errno.h"

static int parse( char* buffer, char **words, int size );


int shell_start( void )
{
    char buffer[SHELL_BUFFER_SIZE];
    int fd, bytes, word_count, i = 0;
    char *words[SHELL_MAX_WORDS];
    char *prompt;
    struct command_s *command;

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
			if( !strcmp( words[0], "exit" ) ) {
				printf("bye...\n");
				return 0;
			}
			else {
				command = command_start();
				while( command ) {
					if( !strcmp( words[0], command->name ) ) {
						(*command->function)( word_count, words );
						break;
					}
					command = command->next;
				}
				if( !command ) {
					printf( "Unknown command\n" );
				}
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
					words[0] = NULL;
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
	/* terminate list */
	words[count] = NULL;

	return count;
}
