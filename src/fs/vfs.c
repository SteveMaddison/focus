#include "focus/vfs.h"
#include "focus/memory.h"
#include "focus/devices.h"
#include "focus/errno.h"
#include "focus/drivers/memory.h"
#include "focus/util.h"

static struct fhandle_s *parse_path( const char* path );
static int save_fhandle( struct fhandle_s *fhandle );
static struct fhandle_s *get_fhandle( int fd );


static struct fhandle_s *fh_array[32];
static int fh_index;


int vfs_init( void )
{

	fh_index = 1;

	if( volume_init() )
		return -1;

	if( mem_dev_init() )
		return -1;

	return 0;
}

/* The following are the syscall functions made available to userland programs.
 * Generally these just fetch the relevant file handle structures and call the
 * driver through the function pointers therein.
 *
 * For open(), read() and write(), errno is set to EACCES if a no function is
 * provided by the driver. For close(), fsync() and fdatasync(), it's assumed
 * nothing has to be done if functions aren't registered and success is returned.
 * It's generally up to the driver to set errno if anything bad happens.
 */

int open( const char *path, int flags )
{
	struct fhandle_s *fhandle;

	if( !(fhandle = parse_path( path )) )
		return -1;
	if( ! fhandle->functions->open ) {
		errno = EACCES;
		return -1;
	}
	if( (*(fhandle->functions->open))( fhandle, flags ) )
		return -1;

	return save_fhandle( fhandle );
}

int close( int fd )
{
	struct fhandle_s *fhandle;

	if( !(fhandle = get_fhandle( fd )) )
		return -1;
	if( ! fhandle->functions->close )
		return 0;
	return (*(fhandle->functions->close))( fhandle );
}

int read( int fd, void *buffer, int count )
{
	struct fhandle_s *fhandle;

	if( !(fhandle = get_fhandle( fd )) )
		return -1;
	if( ! fhandle->functions->read ) {
		errno = EACCES;
		return -1;
	}
	return (*(fhandle->functions->read))( fhandle, buffer, count );
}

int write( int fd, const void* buffer, int count )
{
	struct fhandle_s *fhandle;

	if( !(fhandle = get_fhandle( fd )) )
		return -1;
	if( ! fhandle->functions->write ) {
		errno = EACCES;
		return -1;
	}
	return (*(fhandle->functions->write))( fhandle, buffer, count );
}

int fsync( int fd ) {
	struct fhandle_s *fhandle;

	if( !(fhandle = get_fhandle( fd )) )
		return -1;
	if( ! fhandle->functions->sync ) {
		return 0;
	}
	return (*(fhandle->functions->sync))( fhandle, SYNC_ALL );
}

int fdatasync( int fd ) {
	struct fhandle_s *fhandle;

	if( !(fhandle = get_fhandle( fd )) )
		return -1;
	if( ! fhandle->functions->sync ) {
		return 0;
	}
	return (*(fhandle->functions->sync))( fhandle, SYNC_DATA );
}

/* Validate an absolute path and return a file handle containing (a pointer
 * to) the device structure, the device number and a normalised path. This
 * is pretty heavy and could most probably be a lot faster if optimised.
 * Thankfully, it's only called when a new file is opened.
 */
static struct fhandle_s *parse_path( const char* path )
{
	char dev_name[ DEV_NAME_LEN ];
	int pos = 0;
	int number = 0;
	struct volume_s* volume;

	errno = EBADPATH; /* early, but this can occur all over the place here */

	for( ; pos < DEV_NAME_LEN - 1 ; pos++ ) {
		if ( path[pos] < 'a' || path[pos] > 'z' )
			break;
		dev_name[ pos ] = path[pos];
	}

	if ( pos == 0 || pos > DEV_NAME_LEN - 1 )
		return NULL;

	dev_name[ pos ] = '\0';
	if ( path[pos] == ':' ) {
		/* no digits - number is zero (as initialised) */
	}
	else {
		if( path[ pos ] < '0' || path[ pos ] > '9' )
			return NULL;
		number = path[ pos++ ] - '0';
		if ( path[pos] != ':' )
			return NULL;
	}
	pos++; /* point to after ':' character */

	printf("name: '%s' no: %d\n", dev_name, number );

	/* check the device exists */
	if ( ! (volume = volume_find( dev_name, number )) )
	{
		printf("No vol\n");
		errno = ENODEV;
		return NULL;
	}
	else {
		struct fhandle_s *fhandle = kmalloc( sizeof( struct fhandle_s ), MEM_KERNEL );
		if ( fhandle ) {
			/* Validate the path part now that we have a buffer into which we
			 * can store the normalised version. This could still fail, in which
			 * case fhandle must be freed.
			 */
			char *dest = &fhandle->path[0];
			char src;

			/* trim leading slashes */
			while( path[ pos ] == '/' )
				pos++;

			while ( src = path[ pos ] ) {
				if ( pos >= PATH_MAX_LEN ) {
					errno = ENAMETOOLONG;
					kfree( fhandle );
					return NULL;
				}
				if(( src >= 'a' && src <= 'z' )
				|| ( src >= '0' && src <= '9' )
				|| ( src >= 'A' && src <= 'Z' )
				|| ( src == '_' )
				|| ( src == '.' )) {
					*dest++ = src;
					pos++;
				}
				else if( src == '/' ) {
					*dest++ = '/';
					while( path[ pos ] == '/' )
						pos++;
				}
				else {
					kfree( fhandle );
					return NULL;
				}
			}

			/* trim trailing slash */
			if( *(dest - 1) == '/' )
				*(dest - 1) = '\0';

			if( volume->fs ) {
				fhandle->functions = volume->fs->functions;
			}
			else {
				fhandle->functions = volume->dev->functions;
			}
			return fhandle;
		}
	}
	return NULL;
}

/* Save a flle handle in the list associated with the current process.
 * Returns an integer file descriptor which can be used by the calling
 * program to identify this file handle.
 */
static int save_fhandle( struct fhandle_s *fhandle )
{
	fh_array[ fh_index ] = fhandle;
	return fh_index++;
}

/* Convert an integer passed by a calling program into a file handle
 * which can be passed on to the drivers.
 */
static struct fhandle_s *get_fhandle( int fd )
{
	return fh_array[ fd ];
}

