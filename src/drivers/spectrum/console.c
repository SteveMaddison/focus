#include "focus/util.h"
#include "focus/spectrum/console.h"
#include "focus/devices.h"
#include "focus/errno.h"
#include "focus/vfs.h"

static int console_open( struct fhandle_s *fhandle, int flags );
static int console_read( struct fhandle_s *fhandle, char* buffer, int bytes );
static int console_write( struct fhandle_s *fhandle, const char* buffer, int bytes );
static int console_close( struct fhandle_s *fhandle );

static struct device_s console_dev;
static struct io_func_s console_func;
static struct volume_s console_vol;

int console_init( void )
{
    console_dev.id = 2;
	strcpy( console_dev.name, "con" );
	console_dev.functions = &console_func;
	console_func.open	= (void*)console_open;
	console_func.close	= (void*)console_close;
	console_func.read	= (void*)console_read;
	console_func.write	= (void*)console_write;
	console_func.sync	= NULL;

    if ( device_register( &console_dev ) == 0 ) {
		console_vol.dev = &console_dev;
		console_vol.number = 0;
		console_vol.root = NULL;
		console_vol.fs = NULL;

		if( volume_register( &console_vol ) == 0 )
			return 0;
	}

	return -1;
}

static int console_open( struct fhandle_s *fhandle, int flags )
{
    UNUSED(flags);
    fhandle->fifo_in = kb_buffer;
    fhandle->fifo_out = fifo_create( CRT_BUFFER_SIZE );
    if( !fhandle->fifo_out ) {
        errno = ENOMEM;
        return -1;
    }

	/* no filesystem on this device! */
	if( fhandle->path[0] ) {
		errno = ENOENT;
		return -1;
	}

	return 0;
}

static int console_read( struct fhandle_s *fhandle, char* buffer, int bytes )
{
    return fifo_read( fhandle->fifo_in, buffer, bytes );
}

static int console_write( struct fhandle_s *fhandle, const char* buffer, int bytes )
{
    return fifo_write( fhandle->fifo_out, buffer, bytes );
}

static int console_close( struct fhandle_s *fhandle )
{
    fifo_destroy( fhandle->fifo_in );
    fifo_destroy( fhandle->fifo_out );
    return 0;
}
