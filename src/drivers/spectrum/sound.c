#include "config.h"
#include "focus/spectrum/sound.h"
#include "focus/spectrum/main.h"
#include "focus/util.h"
#include "focus/errno.h"

int sound_init( void )
{
	silence();
	return 0;
}

void silence( void )
{
	/* Reset the speaker bit of port 254. The rest is saved as
	 * this port also controls the microphone and screen border.
	 */
	set_port_fe( 0, 0x10 );
}

void beep( void )
{
	int i;
	set_port_fe( 0x10, 0x10 );
	for( i = 0 ; i < 5 ; i++ );
	silence();
}
