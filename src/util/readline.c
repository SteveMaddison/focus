#include "focus/util.h"
#include "focus/spectrum/keyboard.h"
#include "focus/spectrum/video.h"
#include "focus/spectrum/sound.h"
#include "focus/vfs.h"

int readline( int fd, char* buffer, int size )
{
	int cur_pos = 0;
	int length = 0;
	int offset = 0;
	char c;
	struct cursor_s cur_start;
	struct cursor_s cur_current;

	get_cursor( &cur_start );

	for(;;) {
		if( read( fd, &c, 1 ) ) {
			switch( c ) {
				case '\n':
					print_char('\n');
					return length;
					break;

				case KB_UP:
					/* history - previous */
					break;

				case KB_DOWN:
					/* history - next */
					break;

				case KB_LEFT:
					if( cur_pos )
						cur_pos--;
					else
						beep();
					break;

				case KB_RIGHT:
					if( cur_pos < length )
						cur_pos++;
					else
						beep();
					break;

				case KB_HOME:
					cur_pos = 0;
					break;

				case KB_END:
					cur_pos = length;
					break;

				case KB_TAB:
					/* auto-complete */
					/*
					if( complete( &buffer[0], &cur_pos, BUFFER_SIZE ) )
						length = strlen( buffer );
					else
					*/
						beep();
					break;

				case '\b':
					if( length && cur_pos ) {
						int tmp = cur_pos - 1;

						if( cur_pos > length ) {
							buffer[ length ] = '\0';
						}
						else {
							int dest = cur_pos - 1;
							while( dest < length ) {
								buffer[ dest ] = buffer[ dest + 1 ];
								dest++;
							}
						}
						cur_pos--;
						length--;

						cur_current.xpos -= 1;
						if( cur_current.xpos < 0 ) {
							cur_current.ypos--;
							cur_current.xpos = SCR_COLS - 1;
						}
						set_cursor( &cur_current );

						while( tmp < length )
							print_char( buffer[ tmp++ ] );
						print_char(' ');
					}
					else {
						beep();
					}
					break;

				case KB_DEL:
					if( length && cur_pos < length ) {
						int tmp = cur_pos;

						if( cur_pos < length ) {
							int dest = cur_pos;
							while( dest < length ) {
								buffer[ dest ] = buffer[ dest + 1 ];
								dest++;
							}
						}
						buffer[ length ] = '\0';
						length--;

						while( tmp < length )
							print_char( buffer[ tmp++ ] );
						print_char(' ');
					}
					else {
						beep();
					}
					break;

				default:
					if( length < size ) {
						int tmp = cur_pos + 1;

						if( length != 0 && cur_pos <= length ) {
							int src = length;
							while( src >= cur_pos ) {
								buffer[ src+1 ] = buffer[ src ];
								src--;
							}
						}
						buffer[ cur_pos ] = c;
						print_char(c);
						cur_pos++;
						length++;
						buffer[ length ] = '\0';
						while( tmp < length )
							print_char( buffer[ tmp++ ] );
					}
					else {
						beep();
					}
					break;
			}

			offset = cur_start.xpos + cur_pos;
			cur_current.ypos = cur_start.ypos;
			while( offset >= SCR_COLS ) {
				offset -= SCR_COLS;
				cur_current.ypos++;
				/* check for scroll */
				if( cur_current.ypos >= SCR_ROWS ) {
					cur_current.ypos = SCR_ROWS - 1;
					cur_start.ypos--;
				}
			}
			cur_current.xpos = offset;
			set_cursor( &cur_current );
		}
	}
}

