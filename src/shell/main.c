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
    /*
	char buffer[ SHELL_BUFFER_SIZE ];

    char c;
    char d;

    fifo_in = fifo_create( 32 );
    if( fifo_in ) {
        printf("> ");
    	for(;;) {
            if( c = kb_scan() ) {
                fifo_write( fifo_in, &c, 1 );
                fifo_read( fifo_in, &d, 1 );
                printf("%c", d );
            }
        }

    }



    {
		printf("> ");
		readline( buffer, SHELL_BUFFER_SIZE );

		if( !strcmp( buffer, "open" ) ) {
			fh = open( "zer:", 0 );
			read( fh, tmp, 20 );
			printf( "%x\n", tmp[0] );
			close( fh );
		}
		else if( !strcmp( buffer, "vols" ) ) {
			struct volume_s *vol = volume_start;
			printf("volume_start: %x\n", vol );
			while( vol ) {
				printf("%x: %s%d:\n", vol, vol->dev->name, vol->number );
				vol = vol->next;
			}
		}
		else if( !strcmp( buffer, "devs" ) ) {
			struct device_s *dev = device_start;
			printf("dev_start: %x (%x)\n", dev, &dev );
			while( dev ) {
				printf("%x: %s:\n", dev, dev->name );
				dev = dev->next;
			}
		}
		else {
			printf( "Unknown command\n" );
		}
	}
	*/
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

