#include "focus/errno.h"

/* Global errno contains the last error encountered */
int errno;

/* Returns an English description of an error number.
 */
const char* strerr( int error_code )
{
	switch( error_code ) {
		case ENOMEM:
			return "Out of memory";
		case ENOENT:
			return "No such file/directory";
		case EACCES:
			return "Permission denied";
		case EEXIST:
			return "File exists";
		case ENOTDIR:
			return "Not a directory";
		case EISDIR:
			return "Is a directory";
		case ENAMETOOLONG:
			return "File name too long";
		case EBADPATH:
			return "Invalid path/device name";
		case ENODEV:
			return "No such device";
		case EDEVREG:
			return "Device already registered";
		case EIO:
			return "I/O error";
		case ENOTBLK:
			return "Not a block device";
		case EBUSY:
			return "Device busy";

		default:
			return "Unknown error";
	}
}

