#include "focus/volume.h"
#include "focus/errno.h"
#include "focus/types.h"
#include "focus/util.h"


struct volume_s *volume_start;


int volume_init( void )
{
	volume_start = NULL;
	return 0;
}

/* Registers a volume. No validation is performed on the volume
 * structure passed, so make sure it's correct!
 */
int volume_register( struct volume_s *volume )
{
	if( volume_find( volume->dev->name, volume->number ) ) {
		errno = EDEVREG;
		return -1;
	}

	volume->next = volume_start;
	volume_start = volume;

	return 0;
}

/* Returns the volume structure associated with the passed name.
 */
struct volume_s *volume_find( const char *name, char number )
{
	struct volume_s *volume = volume_start;

	while( volume ) {
		if( !strcmp( volume->dev->name, name ) && volume->number == number )
			return volume;
		volume = volume->next;
	}

	errno = ENODEV;
	return NULL;
}

struct volume_s *volume_first( void )
{
	return volume_start;
}
