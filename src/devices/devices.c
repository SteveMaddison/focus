#include "focus/devices.h"
#include "focus/errno.h"
#include "focus/types.h"
#include "focus/util.h"


/* Start of the devices linked list.
 * The nodes are actually defined in the device drivers themselves.
 */
struct device_s *device_start;


int device_init( void )
{
	device_start = NULL;
	return 0;
}

/* Registers a device driver. No validation is performed on the device
 * structure passed, so make sure it's correct!
 */
int device_register( struct device_s *dev )
{
	if( device_find( dev->name ) ) {
		errno = EDEVREG;
		return -1;
	}

	dev->next = device_start;
	device_start = dev;

	return 0;
}

/* Returns the device structure associated with the passed name.
 */
struct device_s *device_find( const char *name )
{
	struct device_s *dev = device_start;

	while( dev ) {
		if( !strcmp( dev->name, name ) )
			return dev;
		dev = dev->next;
	}

	errno = ENODEV;
	return NULL;
}
