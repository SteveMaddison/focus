/*
 * Drivers for the "bit bucket" devices, nul0-9: and zer0-9:
 * Both disgard any data written to them.
 * Reading the zer: devices always results in requested number of '\0' characters.
 * Reading the nul: devices always returns end of file (0).
 */

#include "focus/devices.h"
#include "focus/errno.h"
#include "focus/vfs.h"
#include "focus/drivers/memory.h"
#include "focus/types.h"
#include "focus/util.h"


static int bb_open( struct fhandle_s *fhandle, int flags );
static int null_read( struct fhandle_s *fhandle, char* buffer, int bytes );
static int zero_read( struct fhandle_s *fhandle, char* buffer, int bytes );
static int bb_write( struct fhandle_s *fhandle, const char* buffer, int bytes );

static struct device_s null_dev;
static struct io_func_s null_func;
static struct volume_s null_vol;

static struct device_s zero_dev;
static struct io_func_s zero_func;
static struct volume_s zero_vol;

int mem_dev_init( void )
{
	null_dev.id = 0;
	strcpy( null_dev.name, "nul" );
	null_dev.functions = &null_func;
	null_func.open	= (void*)bb_open;
	null_func.close	= NULL;
	null_func.read	= (void*)null_read;
	null_func.write	= (void*)bb_write;
	null_func.sync	= NULL;

	zero_dev.id = 1;
	strcpy( zero_dev.name, "zer" );
	zero_dev.functions = &zero_func;
	zero_func.open	= (void*)bb_open;
	zero_func.close	= NULL;
	zero_func.read	= (void*)zero_read;
	zero_func.write	= (void*)bb_write;
	zero_func.sync	= NULL;

	if ( device_register( &null_dev ) == 0 && device_register( &zero_dev ) == 0 ) {
		null_vol.dev = &null_dev;
		null_vol.number = 0;
		null_vol.root = NULL;
		null_vol.fs = NULL;

		zero_vol.dev = &zero_dev;
		zero_vol.number = 0;
		zero_vol.root = NULL;
		zero_vol.fs = NULL;

		if( volume_register( &null_vol ) == 0 && volume_register( &zero_vol ) == 0 )
			return 0;
	}

	return -1;
}

static int bb_open( struct fhandle_s *fhandle, int flags )
{
    UNUSED(flags);
	/* no filesystem on this device! */
	if( fhandle->path[0] ) {
		errno = ENOENT;
		return -1;
	}

	return 0;
}

static int null_read( struct fhandle_s *fhandle, char* buffer, int bytes )
{
    UNUSED(fhandle);
    UNUSED(buffer);
    UNUSED(bytes);
	return 0;
}

static int zero_read( struct fhandle_s *fhandle, char* buffer, int bytes )
{
    UNUSED(fhandle);
	memset( buffer, '\0', bytes );
	return bytes;
}

static int bb_write( struct fhandle_s *fhandle, const char* buffer, int bytes )
{
    UNUSED(fhandle);
    UNUSED(buffer);
	return bytes;
}
