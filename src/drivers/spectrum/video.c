#include "config.h"
#include "focus/spectrum/video.h"
#include "focus/spectrum/main.h"
#include "focus/font.h"
#include "focus/util.h"
#include "focus/errno.h"


static char* cursor_addr( const char x, const char y );
static char* attrib_addr( const char x, const char y );

static char* last_cursor_addr;
static struct cursor_s cursor;
static char attrib;


int video_init( void )
{
	attrib = ATTR_DEFAULT;	/* set colours */
	cls(); 					/* clear the screen */
	set_border( BORDER_DEFAULT );

	return 0;
}

static char* cursor_addr( const char x, const char y )
{
	int third, row, col;

	third 	= y & 0x18;
	row		= y & 0x07;
	col		= x & 0x1f;

	return ( col | (row << 5) | (third << 8) | VRAM_START );
}

static char* attrib_addr( const char x, const char y )
{
	return ARAM_START + (y * SCR_COLS) + x;
}

static void display_cursor( void )
{
	*last_cursor_addr &= ~ATTR_FLASH;
	last_cursor_addr = attrib_addr( cursor.xpos, cursor.ypos );
	*last_cursor_addr |= ATTR_FLASH;
}

static void hide_cursor( void )
{
    char *addr = attrib_addr( cursor.xpos, cursor.ypos );
    *addr &= ~ATTR_FLASH;
}

void cls( void )
{
	char *addr;

	/* clear pixels */
  	for ( addr = (char*)VRAM_START ; addr < (char*)ARAM_START ; addr ++ ) {
		*addr =	0;
	}
	/* set attributes */
	for	( addr = (char*)ARAM_START ; addr <= (char*)VRAM_END ; addr ++ ) {
		*addr = attrib;
	}

	/* initialise cursor position */
	cursor.xpos = 0;
	cursor.ypos = 0;
	last_cursor_addr = cursor_addr( cursor.xpos, cursor.ypos );
}

void get_cursor( struct cursor_s *cur )
{
	cur->xpos = cursor.xpos;
	cur->ypos = cursor.ypos;
}

void set_cursor( struct cursor_s *new_cur )
{
	last_cursor_addr = attrib_addr( cursor.xpos, cursor.ypos );

	cursor.xpos = new_cur->xpos;
	cursor.ypos = new_cur->ypos;

	display_cursor();
}

void set_border( const char colour )
{
	/* The border is set by changing the last 3 bits of the value
	 * in port 254. We want to preserve the rest as these control the
	 * speaker and microphone.
	 */
	set_port_fe( colour, 0x07 );
}

void set_attrib( const char value )
{
	attrib = value;
}

char get_attrib( void )
{
	return attrib;
}

void plot( const char x, const char y )
{
	int xp, yp, xr, yr;
	char* p;

	xr = x & 0x03;
	xp = x >> 3;
	yr = x & 0x03;
	yp = y >> 3;
	yr *= 256;

	p = cursor_addr( xp, yp );
	/*p += yr;
	*p = 128;*/

}

void print_string( const char* string )
{
	char* p = string;

	while( *p )
		print_char( *p++ );

}

void print_char( const char letter )
{
	if ( letter < 1 || letter > 127 )
		return;

    hide_cursor();
	last_cursor_addr = attrib_addr( cursor.xpos, cursor.ypos );

	switch( letter ) {
		case 7:
			/* bell */
			break;
		case '\b':
			cursor.xpos--;
			break;
		case '\n':
			cursor.ypos++;
		case '\r':
			cursor.xpos = 0;
			break;

		default:
		{
			char* addr = cursor_addr(cursor.xpos,cursor.ypos);
			int i;
			char* bitmap = &charset[ ((letter - 32) * 8) ];

			for ( i = 0 ; i < 8 ; i++ ) {
				*addr = *bitmap++;
				addr += 256;
			}

			addr = attrib_addr(cursor.xpos,cursor.ypos);
			*addr = attrib;

			cursor.xpos++;
		}
	}

	if ( cursor.xpos >= SCR_COLS ) {
		cursor.ypos++;
		cursor.xpos = 0;
	}
	if ( cursor.ypos >= SCR_ROWS ) {
		last_cursor_addr -= SCR_ROWS - 1;
		scroll();
		cursor.ypos = SCR_ROWS - 1;
	}
	if( cursor.xpos < 0 ) {
		if ( cursor.ypos != 0 ) {
			cursor.xpos = SCR_COLS - 1;
			cursor.ypos--;
		}
	}
	display_cursor();
}

void scroll( void )
{
	char *addr;
	size_t third_offset, row_offset;
	size_t block_start;
	size_t block_size = ROW_SIZE - SCR_COLS;

	/* scroll the attributes */
	memcpy(	(void*)ARAM_START, (void*)(ARAM_START+SCR_COLS), 768 - SCR_COLS );

	/* add new row of attributes */
	for( addr = (char*)VRAM_END-SCR_COLS+1 ; addr <= (void*)VRAM_END ; addr++ )
		*addr = attrib;

	/* shift rows in video RAM up one */
	for( third_offset = 0 ; third_offset < 0x1800 ; third_offset += THIRD_SIZE ) {
		for( row_offset = 0 ; row_offset < THIRD_SIZE ; row_offset += ROW_SIZE ) {
			block_start = VRAM_START + third_offset + row_offset;
			
			memcpy( (void*)block_start, (void*)(block_start + SCR_COLS), block_size );
			
			if( third_offset < THIRD_SIZE * 2 )
				memcpy( (void*)(block_start + block_size), (void*)(block_start + THIRD_SIZE), SCR_COLS );
			else
				memset( (void*)(block_start + block_size), 0, SCR_COLS );
		}
	}
}
