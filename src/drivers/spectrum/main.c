#include "focus/spectrum/main.h"
#include "focus/spectrum/sound.h"
#include "focus/spectrum/video.h"
#include "focus/spectrum/keyboard.h"
#include "focus/util.h"

int hw_init()
{
	set_port_fe( 0, 0xff );

	if( video_init() )
		return 1;

	if( sound_init() )
		return 1;

	return 0;
}

void set_port_fe( char value, char mask )
{
	static char saved;

	saved &= ~mask;
	saved |= (value & mask);
	outb( 0xfe, saved );
}
